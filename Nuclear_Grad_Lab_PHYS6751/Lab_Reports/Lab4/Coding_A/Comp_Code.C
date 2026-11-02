#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <string>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TString.h>
#include <TMath.h>
#include <vector>
#include <string>
#include <TLegend.h>
#include <TLatex.h>
#include <cmath>
using namespace std;

/* 
      Algorithm for compistion code main measurements 
      
      
 1. Silver, Gold and Copper theoretical cross sections. 
    * use Nuclear derived Rutherford formula (Function) 
    
 2. Calculate n_ag/n_au,  n_cu/n_au,  n_au/n_ag, and n_cu/n_ag (use predefined formulas and counts as functions to get those) 
 
 
 3. Calculate Gold Comp with predefined function, Silver with same thing, and Copper do 1 - GC - SC = Copper composition 
 
 
* Tag 1061 data. 
 
 */ 
 
 
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
    double d_theta_rad = 1.0 * pi / 180.0; // 1-degree uncertainty in radians
    double d_sin_term = (-2.0 * factor * pow(Z_ratio, 2) * cos(theta_rad / 2) )/ (pow(sin(theta_rad / 2), 5)  );
    d_sigma_error =  sqrt(pow(d_sin_term, 2) * pow(d_theta_rad,2));
    cout << "Cross Section: " << d_sigma_d_Omega << " ± " << d_sigma_error << " mb/sr" << endl;
    return d_sigma_d_Omega;
}

// Ratio calculation function with error propagation
double PartialRatios(double N_1, double N_2, double D_1, double D_2, double d_N1, double d_N2, double d_D1, double d_D2, double& ratio_error) {
    double R_Ratio = (N_1 / N_2) * (D_2 / D_1);

    // Error propagation for ratios
    double term1 = (D_2 / D_1) * (1.0 / N_2);
    double term2 = -1.0 * ((N_1 * D_2)/(pow(N_2,2) * D_1));
    double term3 = (N_1 / N_2) * (1.0 / D_1);
    double term4 = -1.0 * ((N_1 * D_2)/(pow(D_1,2) * N_2));
    ratio_error = sqrt(pow(term1, 2)*pow(d_N1,2) + pow(term2, 2)*pow(d_N2,2) + pow(term3, 2)*d_D2*d_D2 + pow(term4, 2)*d_D1*d_D1);

    cout << "Partial Ratio: " << R_Ratio << " ± " << ratio_error << endl;
    return R_Ratio;
}

// Composition calculation function with error propagation
double Composition(double R_2_1, double R_3_1, double d_R21, double d_R31, double& comp_error) {
    double Comp_R = 1.0 / (1.0 + R_2_1 + R_3_1);
    // Error propagation for composition
    double sigma_R_21 = (-1.0/pow((1 + R_2_1 + R_3_1),2));
    comp_error = sqrt(pow(sigma_R_21,2) * (pow(d_R21, 2) + pow(d_R31, 2)));
    cout << "Composition: " << Comp_R << " ± " << comp_error << endl;
    return Comp_R;
}

// Main algorithm function
void main_alg() {
    const double pi = 3.141592653589793;
    const double E_i = 6.5;            // Initial energy in MeV
    const double alpha_N = 2.0;        // Atomic number for alpha particle
    const double M_o = 4.002602;       // Alpha particle mass (g/mol)
    const double d_Mo = 0.000002;         // Uncertainty in M_o (example)

    // Atomic numbers and masses for gold, silver, copper
    const vector<double> atomic_N = {79.0, 47.0, 29.0};
    const vector<double> M_1 = {196.966570, 107.8682, 63.546}; // Atomic masses in g/mol
    const vector<double> M1Errors = {0.000004, 0.0002, 0.003};      // Uncertainties in M_1

    // Experimental counts
    const vector<double> Counts = {4458.09, 218.274, 73.1709};
    vector<double> CountsErrors; // Uncertainty is sqrt(Counts)
    for (double count : Counts) {
        CountsErrors.push_back(sqrt(count));
    }

    // Scattering angle and its uncertainty
    const double thetaDegrees = 155.0;

    // Calculate differential cross-sections with uncertainties
    vector<double> crossSections, crossSectionsErrors;
    for (size_t i = 0; i < atomic_N.size(); i++) {
        double d_sigma_error = 0.0;
        double sigma = RutherfordCrossSection(thetaDegrees, E_i, atomic_N[i], alpha_N, M_o, M_1[i], d_sigma_error);
        crossSections.push_back(sigma);
        crossSectionsErrors.push_back(d_sigma_error);
    }

    // Calculate partial ratios with uncertainties
    double n_ag_n_au_error, n_cu_n_au_error, n_au_n_ag_error, n_cu_n_ag_error, n_ag_n_cu_error, n_au_n_cu_error;
    double n_ag_n_au = PartialRatios(Counts[1], Counts[0], crossSections[1], crossSections[0],
                                     CountsErrors[1], CountsErrors[0], crossSectionsErrors[1], crossSectionsErrors[0], n_ag_n_au_error);
    double n_cu_n_au = PartialRatios(Counts[2], Counts[0], crossSections[2], crossSections[0],
                                     CountsErrors[2], CountsErrors[0], crossSectionsErrors[2], crossSectionsErrors[0], n_cu_n_au_error);
    double n_au_n_ag = PartialRatios(Counts[0], Counts[1], crossSections[0], crossSections[1],
                                     CountsErrors[0], CountsErrors[1], crossSectionsErrors[0], crossSectionsErrors[1], n_au_n_ag_error);
    double n_cu_n_ag = PartialRatios(Counts[2], Counts[1], crossSections[2], crossSections[1],
                                     CountsErrors[2], CountsErrors[1], crossSectionsErrors[2], crossSectionsErrors[1], n_cu_n_ag_error);
                                     
  double n_ag_n_cu = PartialRatios(Counts[1], Counts[2], crossSections[1], crossSections[2], CountsErrors[1], CountsErrors[1], crossSectionsErrors[1], crossSectionsErrors[2], n_ag_n_cu_error);
                                     
                                     
  double n_au_n_cu = PartialRatios(Counts[0], Counts[2], crossSections[0], crossSections[2],
                                     CountsErrors[0], CountsErrors[2], crossSectionsErrors[0], crossSectionsErrors[2], n_au_n_cu_error);

    // Calculate compositions with uncertainties
    double goldCompError, silverCompError, copperCompError;
    double goldComp = Composition(n_ag_n_au, n_cu_n_au, n_ag_n_au_error, n_cu_n_au_error, goldCompError);
    double silverComp = Composition(n_au_n_ag, n_cu_n_ag, n_au_n_ag_error, n_cu_n_ag_error, silverCompError);
    double copperComp = Composition(n_ag_n_cu, n_au_n_cu, n_ag_n_cu_error, n_au_n_cu_error, copperCompError);

    // Output results
    cout << "Gold Composition: " << goldComp * 100.0 << " ± " << goldCompError * 100.0 << "%" << endl;
    cout << "Silver Composition: " << silverComp * 100.0 << " ± " << silverCompError * 100.0 << "%" << endl;
    cout << "Copper Composition: " << copperComp * 100.0 << " ± " << copperCompError * 100.0 << "%" << endl;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
