#include <iostream> 
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TLatex.h>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <vector>
#include <TMatrixD.h>
#include <TVirtualFitter.h>
#include <TLegend.h>
#include <TLine.h>
#include <cmath>
using namespace std; 

void Energy_Calibration(TCanvas* ECalibration, TCanvas* ResidualCanvas) {
    ECalibration->cd(); 

    // Known energies in (MeV) and corresponding channel numbers and std deviations
    Double_t Energies[8] = {2.28291, 2.43128,  3.42124, 3.21245, 4.14071, 4.40983, 6.38506, 5.99539};
    Double_t Channel_N[8]  = {183.098, 196.004, 280.418, 262.462, 340.914, 364.018, 529.784, 497.239};
    Double_t Channel_err[8] = {0.0307144, 0.0049933, 0.00457046, 0.0362986, 0.0403666, 0.0205744, 0.0186997, 0.032646};
    Double_t Energies_err[8] = {0.000670745, 0.00129482, 0.00182204, 0.000943856, 0.00121659, 0.00234853, 0.00340047, 0.00176151};
    int n = 8;

    // Create TGraphErrors
    TGraphErrors* graph = new TGraphErrors(n, Channel_N, Energies, Channel_err, Energies_err);

    // Fit a linear function
    TF1 *Calibration = new TF1("Calibration", "[0]*x + [1]", 0, 8000); 
    TFitResultPtr result = graph->Fit(Calibration, "S");

    // Obtain fit parameters and errors
    double a = Calibration->GetParameter(0);
    double b = Calibration->GetParameter(1);
    double sigma_a = Calibration->GetParError(0);
    double sigma_b = Calibration->GetParError(1);

    // Covariance matrix
    TMatrixDSym cov = result->GetCovarianceMatrix();
    cov.Print();
    double Cov_ab = cov(0, 1); // Covariance between slope and intercept

    // Print calibration parameters
    cout << "Slope (a): " << a << " ± " << sigma_a << endl;
    cout << "Intercept (b): " << b << " ± " << sigma_b << endl;
    cout << "Covariance Cov(a, b): " << Cov_ab << endl;

    // Calibration error and new energies
    vector<double> New_Energies(8);
    vector<double> Calibration_error(8);

    for (int i = 0; i < n; i++) {
        // Calculate new energy using calibration
        New_Energies[i] = a * Channel_N[i] + b;

        // Calculate calibration error
        Calibration_error[i] = sqrt(sigma_a * sigma_a * Channel_N[i] * Channel_N[i] + sigma_b * sigma_b + 2 * Channel_N[i] * Cov_ab);

        // Print new energies and calibration errors
        cout << "Channel " << Channel_N[i]
             << ": New Energy = " << New_Energies[i]
             << ", Calibration Error = " << Calibration_error[i] << endl;
    }

    // Residual calculations
    vector<double> Residuals(8);
    vector<double> Residual_Unc(8);

    for (int i = 0; i < n; i++) {
        // Calculate residual
        Residuals[i] = Energies[i] - New_Energies[i];

        // Residual uncertainty
        Residual_Unc[i] = sqrt(Energies_err[i]  +  Calibration_error[i]);

        // Print residuals and uncertainties
        cout << "Channel " << Channel_N[i]
             << ": Residual = " << Residuals[i]
             << " ± " << Residual_Unc[i] << endl;
    }

    // Plot calibration graph
    graph->SetTitle("Energy Calibration");
    graph->Draw("AP");
    graph->SetMarkerStyle(21);
    graph->SetMarkerSize(1.0);
    graph->SetMarkerColor(kBlue);
    graph->SetLineColor(15);
    graph->SetLineWidth(7);

    graph->GetXaxis()->SetTitle("Channel Number (Arb. Unit)");
    graph->GetYaxis()->SetTitle("E (MeV)");

    // Residual plot
    ResidualCanvas->cd();
    TGraphErrors* residualGraph = new TGraphErrors(n, Energies, Residuals.data(), Energies_err, Residual_Unc.data());
    residualGraph->SetTitle("Residuals vs Original Energies");
    residualGraph->SetMarkerStyle(21);
    residualGraph->SetMarkerSize(1.0);
    residualGraph->SetMarkerColor(kRed);

    residualGraph->GetXaxis()->SetTitle("Original Energy (MeV)");
    residualGraph->GetYaxis()->SetTitle("Residuals (MeV)");
    residualGraph->Draw("AP");

    // Draw a horizontal line at y = 0 using TLine
    TLine* zeroLine = new TLine(2, 0, 6.5, 0); // Line spans the range of x-values
    zeroLine->SetLineColor(kBlack);
    zeroLine->SetLineWidth(2);
    zeroLine->Draw("SAME");

    // Write to ROOT file
    TFile *Histo_data = new TFile("F_D-Rutherford_Calibrations.root", "RECREATE");
    ECalibration->Write();
    ResidualCanvas->Write();
    Histo_data->Close();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1");
    TCanvas *c2 = new TCanvas("c2");
    Energy_Calibration(c1, c2);
}




































































































































