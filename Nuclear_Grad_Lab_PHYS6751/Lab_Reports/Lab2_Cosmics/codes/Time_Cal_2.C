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
#include <cmath>

using namespace std; 

void Time_Calibration (TCanvas* tCalibration) {
    tCalibration->cd(); 
    Double_t Delays[5] = {0.44, 1.50, 2.80, 4.0 , 5.14};
    Double_t Channel_N[5]  = {1.48000e+03, 2.64755e+03, 3.81093e+03, 4.96768e+03, 6.06331e+03};
    Double_t C_err[5] = {0.040623, 0.143736, 0.598185,  1.15945,  0.786362}; 
    Double_t delay_err[5] = {0.1, 0.1, 0.1, 0.1, 0.1}; 
    int n = 5;

    // Create TGraphErrors for y-errors
    TGraphErrors* graphY = new TGraphErrors(n, Channel_N, Delays, 0, delay_err);
    graphY->SetTitle("Time Delay Calibration");
    graphY->SetMarkerStyle(21);
    graphY->SetMarkerSize(1.0);
    graphY->SetMarkerColor(kBlue);
    graphY->SetLineColor(kBlue); // Set y-error bars color
    graphY->Draw("AP"); // Draw with points and y-error bars

    // Create TGraphErrors for x-errors
    TGraphErrors* graphX = new TGraphErrors(n, Channel_N, Delays, C_err, 0);
    graphX->SetMarkerStyle(21);
    graphX->SetMarkerSize(1.0);
    graphX->SetMarkerColor(kBlue);
    graphX->SetLineColor(kRed); // Set x-error bars color
    graphX->Draw("P SAME"); // Draw with points and x-error bars on the same canvas

    // Fit a linear function
    TF1 *Calibration = new TF1("Calibration", "[0]*x + [1]", 1000, 8000); 
    graphY->Fit(Calibration, "R"); // Fit using y-errors

    // Adjust the axis labels
    graphY->GetXaxis()->SetTitleOffset(0.95);
    graphY->GetXaxis()->SetTitleSize(0.05);
    graphY->GetYaxis()->SetTitleOffset(0.95);
    graphY->GetYaxis()->SetTitleSize(0.05);

    // Set the X-axis and Y-axis labels
    graphY->GetXaxis()->SetTitle("Mean Bin (arb. unit)");
    graphY->GetYaxis()->SetTitle("Time Delay (#mus)");

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.1, 0.7, 0.4, 0.9);
    legend->AddEntry(graphY, "Data points (Y-errors)", "p");
    legend->AddEntry(graphX, "X-error bars", "l");
    legend->AddEntry(Calibration, "Linear Fit", "l");
    legend->Draw();
    
    
    
    double a = 0.001036; 
    double b = -1.15;
    vector <Double_t> Delay_C(5);
    for(int i = 0;  i < 5; i++) 
    {
       Delay_C[i] = (a*Channel_N[i] + b);
       cout << "The delay count calculation: " << Delay_C[i] << endl; 
       
    }
    
    
    
    vector <Double_t> Res(5);
    vector <Double_t> Res_err(5); 
    for(int k = 0;  k < 5; k++) 
    {
      Res[k] = (Delays[k] - Delay_C[k]); 
      Res_err[k] = sqrt(Delays[k] + Delay_C[k]); 
      cout << "Residual = " << Res[k] << " +/- " << Res_err[k] << endl; 
       
    }
    
    
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    Time_Calibration(c1);
    TFile *Histo_data = new TFile("Time_Calibrations.root", "RECREATE"); 
    c1->Write();
    Histo_data->Close(); 
}



