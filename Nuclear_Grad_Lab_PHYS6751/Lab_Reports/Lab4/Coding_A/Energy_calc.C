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
#include <string>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <cmath>
using namespace std; 


/* 

                   Energy Calculation Process (Have Cout statements everywhere) 

1. E_att = (Mass Stopping power) * (molar mass / Avg Number) * Comp % * Target Density 

2. E_f = E_i - E_att  (Energy Loss to foil)

3. E_avg = (E_i + E_f)/2 (Use for kinetic energy transformation

K = [(M_o*cos(theta) + sqrt(M_1^2 - M_o^2*sin^2(theta)))/(M_o + M_1)]^2
     M_o -> Source mass (alphas),   M_1 -> Target mass (gold), theta -> Scattering angles (Det angles) 

4. K*E_avg = E_sc  (Units: MeV) 



*/ 



/* 


* Theta_det = 155 deg
  theta targ = 0 deg
  Theta_det = 50 deg
  theta targ = 25 deg         (Forward Scattering and Backwards Data) 
  
  
  * PRD detector (tag#0's)
  
  
* tags:  20's, 40's, 1010's, 1020's, 1030's, 1040's, 1050's, 1060's  


* Energy initial: 2.5, 3.5, 4.5, 6.5 Mev 

* M_1 = 196.966 570(4) u (gold) (g/mol) 

* 1amu = 1.66053906660×10 ^−27 kg.

*  1 kg = 1000g 

*  M_o = 4.002 602(2) u (alphas) (g/mol)

*  Pub Chem source. 

* Error analysis with PRD angles uniformally assume 1 degree. 
* gold analysis right now 

  


*/ 



// Function to convert degrees to radians
double degreesToRadians(double thetaDegrees) {
    const double pi = 3.141592653589793;
    double thetaRadians = thetaDegrees * pi / 180.0;
    cout << "Theta in radians: " << thetaRadians << endl;
    return thetaRadians;
}


// Function to calculate E_att
double calculate_E_att(double massStoppingPower, double molarMass, double compositionFraction, double targetDensity, double AvogadroNumber) {
    double E_att = massStoppingPower * (molarMass / AvogadroNumber) * compositionFraction * targetDensity;
    cout << "E_att (Energy attenuated): " << E_att << " MeV" << endl;
    return E_att;
}

// Function to calculate E_f
double calculate_E_f(double E_i, double E_att) {
    double E_f = E_i - E_att;
    cout << "E_f (Energy after foil): " << E_f << " MeV" << endl;
    return E_f;
}

// Function to calculate E_avg
double calculate_E_avg(double E_i, double E_f) {
    double E_avg = (E_i + E_f) / 2.0;
    cout << "E_avg (Average energy): " << E_avg << " MeV" << endl;
    return E_avg;
}

// Function to calculate K
double calculate_K(double M_o, double M_1, double thetaRadians) {
    double cosTheta = cos(thetaRadians);
    double sinTheta = sin(thetaRadians);
    double K = pow((M_o * cosTheta + sqrt(pow(M_1, 2) - pow(M_o * sinTheta, 2))) / (M_o + M_1), 2);
    cout << "K (Kinetic energy transformation factor): " << K << endl;
    return K;
}

// Function to calculate E_sc
double calculate_E_sc(double K, double E_avg) {
    double E_sc = K * E_avg;
    cout << "E_sc (Scattered energy): " << E_sc << " MeV" << endl;
    return E_sc;
}


// Function to calculate uncertainty in K and E_sc
void calculate_uncertainties(double K, double M_o, double M_1, double thetaRadians, double unc_thetaRadians, double unc_M_o, double unc_M_1,
                             double E_sc, double unc_K, double unc_E_sc) {
    // Uncertainty in K
    double dK_theta = fabs(-2 * K * M_o * sin(thetaRadians) / (M_o + M_1)) * unc_thetaRadians;
    double dK_M_o = fabs((2 * K / (M_o + M_1)) * (1 / M_o)) * unc_M_o;
    double dK_M_1 = fabs((2 * K / (M_o + M_1)) * (1 / M_1)) * unc_M_1;
    unc_K = sqrt(dK_theta * dK_theta + dK_M_o * dK_M_o + dK_M_1 * dK_M_1);

    cout << "Uncertainty in K: " << unc_K << endl;

    // Uncertainty in E_sc
    unc_E_sc = E_sc * sqrt(pow(unc_K / K, 2));
    cout << "Uncertainty in E_sc: " << unc_E_sc << " MeV" << endl;
}







void Energy_calculation() 

{
// Constants
    double A_N = 6.022e23;                 // particles/mol
    const double pi_ex = 3.141592653589793;
    double massStoppingPower[4] = {3.168e2, 2.744e2, 2.453e2, 2.060e2}; // MeV cm^2/g            
    double cF = 0.8;                       // Composition fraction (%)
    double targetDensity = 8e17;           // particles/cm^2
    double E_i[4] = {2.5, 3.5, 4.5, 6.5};  // Initial energy in MeV
    double M_o = 4.002602;                 // Source mass (alphas) in g/mol
    double M_1 = 196.966570;               // Target mass (gold) in g/mol
    double thetaDegrees[2] = {168.0, 168.0};  // Scattering angles in degrees

    // Uncertainties
    double unc_thetaDegrees = 1.0;         // 1 degree
    double unc_thetaRadians = unc_thetaDegrees * pi_ex / 180.0; // Convert uncertainty to radians
    double unc_M_o = 0.000002;             // Uncertainty in M_o
    double unc_M_1 = 0.000004;             // Uncertainty in M_1

    cout << "Begin Energy Calculation Process\n" << endl;

    // Loop through the tags and calculate
    struct TagInfo {
        string name;
        int energyIndex;  // Corresponds to E_i and massStoppingPower
        int angleIndex;   // Corresponds to thetaDegrees
    };

    vector<TagInfo> tags = {
        {"20's", 0, 0}, {"40's", 0, 1}, {"1010's", 1, 1},
        {"1020's", 1, 0}, {"1030's", 2, 0}, {"1040's", 2, 1},
        {"1050's", 3, 1}, {"1060's", 3, 0}
    };

    for (const auto& tag : tags) {
        cout << "\nTag " << tag.name << ": Information" << endl;

        // Angle in radians
        double thetaRadians = degreesToRadians(thetaDegrees[tag.angleIndex]);

        // Energy Attenuation
        double E_att = calculate_E_att(massStoppingPower[tag.energyIndex], M_1, cF, targetDensity, A_N);

        // Final Energy after foil
        double E_f = calculate_E_f(E_i[tag.energyIndex], E_att);

        // Average Energy
        double E_avg = calculate_E_avg(E_i[tag.energyIndex], E_f);

        // Kinetic Energy Transformation Factor
        double K = calculate_K(M_o, M_1, thetaRadians);

        // Scattered Energy
        double E_sc = calculate_E_sc(K, E_avg);

        // Uncertainties in K and E_sc
        double unc_K = 0.0, unc_E_sc = 0.0;
        double dK_theta = fabs(-2 * K * M_o * sin(thetaRadians) / (M_o + M_1)) * unc_thetaRadians;
        double dK_M_o = fabs((2 * K / (M_o + M_1)) * (1 / M_o)) * unc_M_o;
        double dK_M_1 = fabs((2 * K / (M_o + M_1)) * (1 / M_1)) * unc_M_1;
        unc_K = sqrt(dK_theta * dK_theta + dK_M_o * dK_M_o + dK_M_1 * dK_M_1);
        unc_E_sc = E_sc * sqrt(pow(unc_K / K, 2));

        cout << "Uncertainty in K: " << unc_K << endl;
        cout << "Uncertainty in E_sc: " << unc_E_sc << " MeV" << endl;
    }

    cout << "\nEnd of Energy Calculation Process\n" << endl;



}














































































































































































