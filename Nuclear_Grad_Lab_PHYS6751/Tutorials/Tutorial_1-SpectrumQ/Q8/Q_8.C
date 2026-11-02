/* 


Q_8. Find the number of events in the two peaks of each the Co-60 and Na-22 calibration spectra and apply a correction for the relative efficiency you measured in (7).  How well do the ratios of the two peaks in each spectrum compare to what you would expect based on literature information; include a citation of your literature source.



*/ 

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

// Function to get efficiency from the energy
double Efficiency(double energy) {
    // Placeholder for your efficiency function
    // For example, if you fit an inverse power law in your Efficiency.C
    return 7.4 / (pow(energy, 0.59));  // Replace with your actual efficiency model
}

void analyze_spectra() {
    // Event counts for Na-22
    double N_Na22_p1 = 9369.0;  // Measured for 511 keV
    double N_Na22_p2 = 2247.0;  // Measured for 1274.53 keV
    
    // Event counts for Co-60
    double N_Co60_p1 = 1497.0;  // Measured for 1173.237 keV
    double N_Co60_p2 = 1175.0;  // Measured for 1332.501 keV	

    // Energy of the peaks
    double E_Na22_p1 = 511;    // keV
    double E_Na22_p2 = 1274.53; // keV
    double E_Co60_p1 = 1173.237; // keV
    double E_Co60_p2 = 1332.501; // keV

    // Apply efficiency correction
    double epsilon_Na22_p1 = Efficiency(E_Na22_p1);
    double epsilon_Na22_p2 = Efficiency(E_Na22_p2);
    double epsilon_Co60_p1 = Efficiency(E_Co60_p1);
    double epsilon_Co60_p2 = Efficiency(E_Co60_p2);

    // Print the efficiency values
    cout << "Efficiency for Na-22 (511 keV): " << epsilon_Na22_p1 << endl;
    cout << "Efficiency for Na-22 (1274.53 keV): " << epsilon_Na22_p2 << endl;
    cout << "Efficiency for Co-60 (1173.237 keV): " << epsilon_Co60_p1 << endl;
    cout << "Efficiency for Co-60 (1332.501 keV): " << epsilon_Co60_p2 << endl;

    // Corrected counts
    double N_Na22_p1_corrected = N_Na22_p1 / epsilon_Na22_p1;
    double N_Na22_p2_corrected = N_Na22_p2 / epsilon_Na22_p2;
    double N_Co60_p1_corrected = N_Co60_p1 / epsilon_Co60_p1;
    double N_Co60_p2_corrected = N_Co60_p2 / epsilon_Co60_p2;

    // Print the corrected counts
    cout << "Corrected Na-22 counts for 511 keV: " << N_Na22_p1_corrected << endl;
    cout << "Corrected Na-22 counts for 1274.53 keV: " << N_Na22_p2_corrected << endl;
    cout << "Corrected Co-60 counts for 1173.237 keV: " << N_Co60_p1_corrected << endl;
    cout << "Corrected Co-60 counts for 1332.501 keV: " << N_Co60_p2_corrected << endl;

    // Calculate ratios
    double Na22_ratio = N_Na22_p2_corrected / N_Na22_p1_corrected;
    double Co60_ratio = N_Co60_p2_corrected / N_Co60_p1_corrected;

    // Print results
    cout << "Na-22 Peak Ratio (1274.53 keV / 511 keV): " << Na22_ratio << endl;
    cout << "Co-60 Peak Ratio (1332.501 keV / 1173.237 keV): " << Co60_ratio << endl;
    cout << "Na-22 Intensity ratio = 0.56" << endl;
    cout << "Co-60 Intensity ratio = 1.0" << endl;
}

























