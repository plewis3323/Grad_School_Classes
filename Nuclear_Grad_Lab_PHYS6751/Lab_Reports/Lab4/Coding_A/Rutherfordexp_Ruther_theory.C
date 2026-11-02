#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <TMath.h>
#include <vector>
#include <TLegend.h>
#include <TLatex.h>
using namespace std;

// Rutherford differential cross-section function with error propagation
double RutherfordCrossSection(double theta, double E0, double Z_0, double Z_1, double M_O, double M_1, double& d_sigma_error) {
    const double pi = 3.141592653589793;
    double theta_rad = theta * pi / 180.0; // Convert angle to radians
    double factor = 1.296;                // mb * MeV^2 / sr
    double Z_ratio = (Z_0 * Z_1) / E0;
    double sin_term = 1.0 / pow(sin(theta_rad / 2), 4);
    double mass_term = -2.0 * pow((M_O / M_1), 2);
    double d_sigma_d_Omega = factor * pow(Z_ratio, 2) * (sin_term + mass_term);

    // Error propagation
    double d_theta_rad = 0.0017453293; // 1-degree uncertainty in radians
    double d_sin_term = (-2.0 * factor * pow(Z_ratio, 2) * cos(theta_rad / 2) )/ (pow(sin(theta_rad / 2), 5)  );
    d_sigma_error =  sqrt(pow(d_sin_term, 2) * pow(d_theta_rad,2));
    cout << "Cross Section: " << d_sigma_d_Omega << " ± " << d_sigma_error << " mb/sr" << endl;
    return d_sigma_d_Omega;
}


// Initial alphas calculation
double Initial_Alphas(double BCI, double Scale) {
    const double Z = 2.0;                // Charge of alpha particle
    const double e = 1.6e-19;            // Elementary charge in C
    double Beam_Charge = (BCI * Scale * 100.0) / 10000.0; // Scale for beam charge
    double N_I = Beam_Charge / (Z * e);  // Number of incident alphas
    cout << "Initial Alphas: " << N_I << " ± " << sqrt(N_I) << endl;
    return N_I;
}

// Experimental Rutherford cross-section with error propagation
double Exp_Rutherford(double N_i, double N_f, double SA, double SA_error, double T_Density, double theta,   double& d_sigma_error) {
    const double pi = 3.141592653589793;
    double theta_rad = theta * pi / 180.0; // Convert angle to radians
    double Exp_C_section = N_f / (SA * N_i * T_Density);

    // Error propagation
    double sig_N_f = sqrt(N_f);   // Poisson statistics
    double sig_N_i = sqrt(N_i);   // Poisson statistics
    double sig_SA = SA_error;     // Solid angle uncertainty
    double dR_dNf = (1.0)/(N_i * SA * T_Density); // differential N_F
    double dR_dNi = (-1.0 * N_f * (SA * T_Density))/(pow((N_i * SA * T_Density),2)); // differential N_i
    double dR_dSA = (-1.0 * N_f * (N_i * T_Density))/(pow((N_i * SA * T_Density),2)); // differential SA
    // target density calculation 
    double sigma_TD = sqrt( pow(((T_Density * 0.792819 * 1e-27 * sin(theta_rad))/(pow(cos(theta_rad), 2)) * 0.0017453293), 2) + pow(((1e-27 * T_Density)/(cos(theta_rad))) * (1.09831 / 100.0), 2));
   double dR_dTD = (-1.0 * N_f * (N_i * SA))/(pow((N_i * SA * T_Density),2)); // differential TD
    
    

    // Total uncertainty
    d_sigma_error = sqrt(pow(dR_dNf,2)*pow(sig_N_f,2) + pow(dR_dNi,2)*pow(sig_N_i,2) + pow(dR_dSA,2)*pow(sig_SA,2)  + pow((dR_dTD * sigma_TD), 2) ); 
    return Exp_C_section;
}

void Main_algorithm() {
    const double pi = 3.141592653589793;
    const double E_i = 2.5;            // Initial energy in MeV
    const double alpha_N = 2.0;        // Atomic number for alpha particle
    const double M_o = 4.002602;       // Alpha particle mass (g/mol)
    const double d_Mo = 0.000002;      // Uncertainty in M_o
    const double targetDensity = 8e17; // particles/cm^2
    const double cF = 0.792819;        // Gold composition fraction
    const double cF_error = 1.09831 / 100.0; // Composition fraction uncertainty
    const double BCI = 5000;           // Beam Charge Intensity

    const vector<double> thetaDegrees = {154.9, 60.0, 50.0, 40.0, 315.0, 305.0, 295.0, 270.0, 245.0, 225.0, 325.0, 30.0};
     const vector<double> thetaDegrees_target = {0.0, 30.0, 25.0, 20.0, 337.5, 332.5, 327.5, 315.0, 315.0, 0.0, 0.0, 342.5, 15.0};
    const vector<double> F_Counts = {9983.66, 18203.4, 55917.3, 55313, 370612, 201769, 119690, 47740.2, 16418.4, 11619.7, 114812, 172453};
    const vector<double> DTL = {1.0465,  1.8391,  5.4118, 7.6136, 25.053, 14.286, 8.4043, 3.2673, 1.5217, 1.1224, 8.1818, 10.28670};

    // Precompute density over cosine
    vector<double> densityOverCosine;
    for (double theta : thetaDegrees_target) {
        double theta_rad = (theta * pi )/ 180.0;
        double cosine = cos(theta_rad);
        densityOverCosine.push_back(cosine != 0.0 ? fabs((targetDensity * cF * 1e-27)/ cosine) : 0.0);
    }

    const double R = (4.01 / 2.0) * 0.1;   // cm
    const double U_d = 0.035;              // cm uncertainty
    const double Area = pi * pow(R, 2);    // cm^2
    const double d = 6.700;                // cm
    const double d_Area = (-2.0 * pi* pow(R,2))/(pow(d,3)); // Uncertainty in area
    const double S_A = Area / pow(d, 2);   // Solid angle (sr)
    const double d_SA = S_A * sqrt(pow(d_Area,2)*pow(U_d,2)); // Uncertainty in solid angle

    vector<double> adjustedAngles, theoreticalCrossSections, experimentalCrossSections, ratioCrossSections;
    vector<double> theoreticalErrors, experimentalErrors, ratioErrors;

    for (size_t i = 0; i < thetaDegrees.size(); ++i) {
        // Adjust angles > 180 degrees
        double adjustedAngle = (thetaDegrees[i] > 180) ? thetaDegrees[i] - 360 : thetaDegrees[i];
        adjustedAngles.push_back(fabs(adjustedAngle));

        // Theoretical cross-section
        double d_sigma_theo;
        double theoretical = RutherfordCrossSection(thetaDegrees[i], E_i, 79.0, alpha_N, M_o, 196.966570, d_sigma_theo);
        theoreticalCrossSections.push_back(theoretical);
        theoreticalErrors.push_back(d_sigma_theo);

        // Corrected counts
        double corrected_count = F_Counts[i] / (1.0 - DTL[i] / 100.0);

        // Initial alphas
        double initial_alphas = Initial_Alphas(BCI, i < 10 ? 1e-8 : 1e-9); // Scale for beam intensity

        // Experimental cross-section
        double d_sigma_exp;
        double experimental = Exp_Rutherford(initial_alphas, corrected_count, S_A, d_SA, densityOverCosine[i], thetaDegrees_target[i], d_sigma_exp);
        experimentalCrossSections.push_back(experimental);
        experimentalErrors.push_back(d_sigma_exp);

        // Ratio and its uncertainty
        double ratio = experimental / theoretical;
        double ratio_error = ratio * sqrt(
            pow(d_sigma_exp / experimental, 2) +
            pow(d_sigma_theo / theoretical, 2)
        );

        ratioCrossSections.push_back(ratio);
        ratioErrors.push_back(ratio_error);

        // Output results
        cout << "Angle: " << thetaDegrees[i] << " degrees" << endl;
        cout << "Adjusted Angle: " << fabs(adjustedAngle) << " degrees" << endl;
        cout << "Theoretical Cross Section: " << theoretical << " ± " << d_sigma_theo << " mb/sr" << endl;
        cout << "Experimental Cross Section: " << experimental << " ± " << d_sigma_exp << " mb/sr" << endl;
        cout << "Ratio (Exp/Theo): " << ratio << " ± " << ratio_error << endl;
        cout << "The solid angle: #omega = " << S_A << endl; 
        cout << "--------------------------------------------------------" << endl;
    }

   // Define the TF1 for sin^-4(x/2) fit
TF1* sinInv4Fit = new TF1("sinInv4Fit", "1/pow(sin([0]*(x/2)), 4)", 20.0, 170.0);
sinInv4Fit->SetLineColor(kMagenta);
sinInv4Fit->SetLineStyle(1);
sinInv4Fit->SetLineWidth(2);

  // Define the TF1 for sin^-4(x/2) fit
TF1* sinInv4Fit2 = new TF1("sinInv4Fit2", "1/pow(sin([0]*(x/2)), 4)", 20.0, 170.0);
sinInv4Fit2->SetLineColor(kCyan);
sinInv4Fit2->SetLineStyle(1);
sinInv4Fit2->SetLineWidth(2);

// Plotting results with TGraphErrors
TGraphErrors* graphTheoretical = new TGraphErrors(adjustedAngles.size(), &adjustedAngles[0], &theoreticalCrossSections[0], 0, &theoreticalErrors[0]);
TGraphErrors* graphExperimental = new TGraphErrors(adjustedAngles.size(), &adjustedAngles[0], &experimentalCrossSections[0], 0, &experimentalErrors[0]);
TGraphErrors* graphRatio = new TGraphErrors(adjustedAngles.size(), &adjustedAngles[0], &ratioCrossSections[0], 0, &ratioErrors[0]);

graphTheoretical->SetLineColor(kRed);
graphTheoretical->SetMarkerColor(kRed);
graphTheoretical->SetMarkerStyle(21);
graphExperimental->SetLineColor(kBlue);
graphExperimental->SetMarkerColor(kBlue);
graphExperimental->SetMarkerStyle(22);
graphRatio->SetLineColor(kGreen);
graphRatio->SetMarkerColor(kGreen);
graphRatio->SetMarkerStyle(20);

// Create a canvas
TCanvas* canvas = new TCanvas("canvas", "Cross Sections vs Angle with Fits", 800, 600);

// Draw theoretical and experimental graphs
graphTheoretical->Draw("AP");
graphExperimental->Draw("P SAME");

// Fit the theoretical data with sin^-4(x/2)
graphTheoretical->Fit("sinInv4Fit", "R");

// Change color of the fit for experimental data and refit
graphExperimental->Fit("sinInv4Fit2", "R");

// Add legend
TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
legend->AddEntry(graphTheoretical, "Theoretical", "p");
legend->AddEntry(graphExperimental, "Experimental", "p");
legend->AddEntry(sinInv4Fit, "sin^{-4}(#theta/2) Fit(theo)", "l");
legend->AddEntry(sinInv4Fit2, "sin^{-4}(#theta/2) Fit(Exp)", "l");
legend->Draw();

// Add titles and labels
graphTheoretical->GetXaxis()->SetTitle("Scattering Angle (degrees)");
graphTheoretical->GetYaxis()->SetTitle("Cross Section (mb/sr)");
graphTheoretical->SetTitle("Theoretical vs. Experimental Cross Sections with Fits");

// Save the canvas
canvas->SaveAs("CrossSections_with_Fits_SameCanvas_BC.root");

TCanvas* canvasRatio = new TCanvas("canvasRatio", "Experimental/Theoretical Ratio", 800, 600);

// Draw the graph first
graphRatio->GetXaxis()->SetTitle("Adjusted Scattering Angle (degrees)");
graphRatio->GetYaxis()->SetTitle("Experimental/Theoretical Ratio");
graphRatio->SetTitle("Ratios graph of Experimental and Theoretical Rutherford");
graphRatio->Draw("AP");

// Draw a horizontal line at y = 1.0 using TLine
TLine* zeroLine = new TLine(20, 1.0, 170, 1.0); // Line spans the range of x-values
zeroLine->SetLineColor(kBlack);
zeroLine->SetLineWidth(2);
zeroLine->Draw("SAME");

// Save the canvas with the added line
canvasRatio->SaveAs("RatioGraph_with_Fit_BC.root");






}







































