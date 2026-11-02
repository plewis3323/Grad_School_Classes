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
#include <cmath>
using namespace std;

// Rutherford differential cross-section function with error propagation
double RutherfordCrossSection(double theta, double E0, double Z_0, double Z_1, double M_O, double M_1, double d_MO, double d_M1, double& d_sigma_error) {
    const double pi = 3.141592653589793;
    double theta_rad = theta * pi / 180.0; // Convert angle to radians
    double factor = 1.296;                // mb * MeV^2 / sr
    double Z_ratio = (Z_0 * Z_1) / E0;
    double sin_term = 1.0 / pow(sin(theta_rad / 2), 4);
    double mass_term = -2.0 * pow((M_O / M_1), 2);
    double d_sigma_d_Omega = factor * pow(Z_ratio, 2) * (sin_term + mass_term);

    // Error propagation
    double d_theta_rad = 1.0 * pi / 180.0; // 1-degree uncertainty in radians
    double d_sin_term = 4 * sin_term * cos(theta_rad / 2) / sin(theta_rad / 2) * d_theta_rad;
    double d_mass_term_MO = 4.0 * mass_term * d_MO / M_O; // Uncertainty from M_O
    double d_mass_term_M1 = 4.0 * mass_term * d_M1 / M_1; // Uncertainty from M_1

    d_sigma_error = factor * pow(Z_ratio, 2) * sqrt(pow(d_sin_term, 2) + pow(d_mass_term_MO, 2) + pow(d_mass_term_M1, 2));
    cout << "Cross Section: " << d_sigma_d_Omega << " ± " << d_sigma_error << " mb/sr" << endl;
    return d_sigma_d_Omega;
}

// Initial alphas function
double Initial_Alphas(double BCI, double Scale) {
    double Z = 2.0; 
    double elect_charge = 1.6e-19; 
    double Beam_Charge = (BCI * Scale * 100.0) / 10000.0; 
    double N_I = Beam_Charge / (Z * elect_charge); 
    cout << "The initial Alphas = " << N_I << endl;
    cout << "The Count error initial alphas = " << sqrt(N_I) << endl; 
    return N_I;
}

// Corrected peaks counts
double Final_alphas(double N, double DTL) {
    double N_corr = N / (1.0 - DTL); 
    cout << "The Corrected Counts = " << N_corr << " ± " << sqrt(N_corr) << endl;
    return N_corr; 
}

// Experimental Rutherford function
double Exp_Rutherford(double N_i, double N_f, double SA, double T_Density) {
    double Exp_C_section = N_f / (SA * N_i * T_Density); 
    cout << "The Experimental Rutherford = " << Exp_C_section << endl; 
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
    const double targetDensity_mb = targetDensity * cF * 1e-27; // Convert to mbarn

    const vector<double> thetaDegrees = {154.9, 60.0, 50.0, 40.0, 215.0, 305.0, 295.0, 270.0, 270.0, 245.0, 225.0, 325.0, 30.0};
    const vector<double> thetaDegrees_target = {0, 60.0 / 2.0, 50.0 / 2.0, 40.0 / 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 30.0 / 2.0};
    const vector<double> F_Counts = {9983.66, 18203.4, 55917.3, 55313, 10675.4, 201769, 119690, 47740.2, 48987.5, 16418.4, 11619.7, 114812, 172453};
    const vector<double> DTL = {1.0465, 1.8391, 5.4118, 7.6136, 25.053, 14.286, 8.4043, 3.2673, 3.8043, 1.5217, 1.1224, 8.1818, 10.28670};

    // Precompute density over cosine
    vector<double> densityOverCosine;
    for (double thetaDeg : thetaDegrees_target) {
        if (thetaDeg == 0.0) {
            densityOverCosine.push_back(targetDensity_mb);
        } else {
            double thetaRad = thetaDeg * pi / 180.0;
            double cosine = cos(thetaRad);
            densityOverCosine.push_back(cosine != 0.0 ? targetDensity_mb / cosine : 0.0);
        }
    }

    // Precompute DTL divided by 100
    vector<double> DTL_divided;
    for (double value : DTL) {
        DTL_divided.push_back(value / 100.0);
    }

    // Containers for results
    vector<double> theoreticalCrossSections;
    vector<double> experimentalCrossSections;
    vector<double> ratioCrossSections;
    vector<double> adjustedAngles;

    const double R = (4.01 / 2.0) * 0.1;   // cm
    const double Area = pi * pow(R, 2);   // cm^2
    const double r = 6.700;               // cm
    const double S_A = Area / pow(r, 2);  // sr
    const double BCI = 5000;              // Beam Charge Intensity

    cout << "Starting main algorithm..." << endl;

    for (size_t i = 0; i < thetaDegrees.size(); ++i) {
        // Adjust angles > 180 degrees
        double adjustedAngle = (thetaDegrees[i] > 180) ? thetaDegrees[i] - 360 : thetaDegrees[i];
        adjustedAngles.push_back(fabs(adjustedAngle));

        // Theoretical cross-section
        double d_sigma_error;
        double theoretical = RutherfordCrossSection(thetaDegrees[i], E_i, 79.0, alpha_N, M_o, 196.966570, d_Mo, 0.000004, d_sigma_error);
        theoreticalCrossSections.push_back(theoretical);

        // Experimental cross-section
        double corrected_count = Final_alphas(F_Counts[i], DTL_divided[i]);
        double initial_alphas = Initial_Alphas(BCI, i < 10 ? 1e-8 : 1e-9);  // Scale depending on angle
        double experimental = Exp_Rutherford(initial_alphas, corrected_count, S_A, densityOverCosine[i]);
        experimentalCrossSections.push_back(experimental);

        // Compute ratio
        ratioCrossSections.push_back(experimental > 0 ? theoretical / experimental : 0.0);

        // Debug output
        cout << "Angle: " << thetaDegrees[i] << " degrees" << endl;
        cout << "Adjusted Angle: " << adjustedAngle << " degrees" << endl;
        cout << "Theoretical Cross-Section: " << theoretical << endl;
        cout << "Experimental Cross-Section: " << experimental << endl;
        cout << "Ratio (Theoretical/Experimental): " << ratioCrossSections.back() << endl;
    }

    // --- Plot for Theoretical vs Experimental ---
    TGraph* graphTheoretical = new TGraph(thetaDegrees.size(), &thetaDegrees[0], &theoreticalCrossSections[0]);
    TGraph* graphExperimental = new TGraph(thetaDegrees.size(), &thetaDegrees[0], &experimentalCrossSections[0]);

    graphTheoretical->SetLineColor(kRed);
    graphTheoretical->SetMarkerStyle(21);
    graphExperimental->SetLineColor(kBlue);
    graphExperimental->SetMarkerStyle(22);

    TCanvas* canvas = new TCanvas("canvas", "Cross Sections vs Angle", 800, 600);
    graphTheoretical->Draw("AP");
    graphExperimental->Draw("P SAME");

    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(graphTheoretical, "Theoretical", "p");
    legend->AddEntry(graphExperimental, "Experimental", "p");
    legend->Draw();

    // Add axis labels
    graphTheoretical->GetXaxis()->SetTitle("Scattering Angle (degrees)");
    graphTheoretical->GetYaxis()->SetTitle("Cross Section (mb/sr)");
    graphTheoretical->SetTitle("Theoretical vs. Experimental Cross Sections");
    canvas->SaveAs("CrossSections.root");

    // --- New Plot for Ratio ---
    TGraph* graphRatio = new TGraph(adjustedAngles.size(), &adjustedAngles[0], &ratioCrossSections[0]);
    graphRatio->SetLineColor(kGreen);
    graphRatio->SetLineWidth(2);
    graphRatio->SetMarkerStyle(20);
    graphRatio->SetMarkerColor(kGreen);
    graphRatio->SetTitle("Theoretical/Experimental Ratio");

    TCanvas* canvasRatio = new TCanvas("canvasRatio", "Theoretical/Experimental Ratio", 800, 600);
    graphRatio->GetXaxis()->SetTitle("Adjusted Scattering Angle (degrees)");
    graphRatio->GetYaxis()->SetTitle("Theoretical/Experimental Ratio");

    graphRatio->Draw("AP");

    TFile* outFileRatio = new TFile("CrossSectionRatios.root", "RECREATE");
    canvasRatio->Write();
    outFileRatio->Close();

    cout << "Ratio graph saved as CrossSectionRatios.root" << endl;
}

















































