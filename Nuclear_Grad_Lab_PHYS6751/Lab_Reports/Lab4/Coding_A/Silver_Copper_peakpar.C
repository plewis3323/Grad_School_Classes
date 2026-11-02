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


/*  Algorithm For getting copper, Silver, Gold Peak positions 

* Using Tag 1061 * 

1.  Make a function for energy Calibration Formula 

2.  Copy and paste functions to get gneral scattering energy/Useful Arrays

3.  Perform the analysis and get the peak positions

4.  Re-apply 1061 peak fitting for proper counts. 


   E_i = 6.5 Mev


Gold: 	196.966570(4)


Silver: 107.8682(2)


Copper: 63.546(3)


Use for citation later: https://iupac.qmul.ac.uk/AtWt/


 

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

double Calibration_Funct(double E) {

  double a = 0.0118092; 
  double b = 0.11717;
  double x = (E-b)/a;
  cout << "The channel number is" << endl; 
  return x; 
   
}

void Main_function() 

{ 


double A_N = 6.022e23;                 // particles/mol
const double pi_ex = 3.141592653589793;
double massStoppingPower[3] = {2.060e2, 2.890e2, 3.699e2}; // MeV cm^2/g  (gold, silver, copper)          
double cF[3] = {0.8, 0.1, 0.1};                       // Composition fraction (%) (gold, silver, copper)
double targetDensity = 8e17;           // particles/cm^2
double E_i = 6.5;  // Initial energy in MeV
double M_o = 4.002602;                 // Source mass (alphas) in g/mol
double M_1[3] = {196.966570, 107.8682 ,63.546};               // Target mass (gold, silver, copper) in g/mol
double thetaDegrees = 155.0;  // Scattering angles in degrees


   // Convert angle to radians
    double thetaRadians = degreesToRadians(thetaDegrees);

    // Loop over materials (gold, silver, copper)
    for (int i = 0; i < 3; i++) {
        // Step 1: Calculate E_att
        double E_att = calculate_E_att(massStoppingPower[i], M_1[i], cF[i], targetDensity, A_N);

        // Step 2: Calculate E_f
        double E_f = calculate_E_f(E_i, E_att);

        // Step 3: Calculate E_avg
        double E_avg = calculate_E_avg(E_i, E_f);

        // Step 4: Calculate K
        double K = calculate_K(M_o, M_1[i], thetaRadians);

        // Step 5: Calculate E_sc
        double E_sc = calculate_E_sc(K, E_avg);

        // Step 6: Get channel number
        double channel = Calibration_Funct(E_sc);

        // Output results
        string material;
        if (i == 0) material = "Gold";
        else if (i == 1) material = "Silver";
        else material = "Copper";

        cout << material << " peak energy (E_sc): " << E_sc << " MeV" << endl;
        cout << material << " channel number: " << channel << endl;
        cout << "----------------------------------------" << endl;
    }









}











































































































































