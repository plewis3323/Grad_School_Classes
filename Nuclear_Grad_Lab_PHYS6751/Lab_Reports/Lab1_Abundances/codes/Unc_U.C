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
#include <vector>
#include <TLegend.h>
#include <cmath>
#include <TLatex.h>
using namespace std; 

double Unc_Calc(double uCounts235, double uCounts238, double Eff235, double Eff238, double BR235, double BR238, double decay235, double decay238, double R, double N1, double N2) {

    double Un_235 = ((uCounts235/Eff235)/(BR235))/(decay235);
    double Un_238 = ((uCounts238/Eff238)/(BR238))/(decay238);
    double Unc_R =  R * sqrt((Un_235/N1)*(Un_235/N1) + (Un_238/N2)*(Un_238/N2));
    
    return Unc_R;
}

void Unc() {

    double R1_Count_atm[2] = {1.09252e+12, 3.94805e+14};
    double R2_Count_atm[2] = {3.05179e+12, 1.08023e+15};
    double R3_Count_atm[2] = {1.9893e+12, 8.22421e+14};

    double R1_unc_Counts[2] = {236.3, 69.7}; 
    double R2_unc_Counts[2] = {369.4, 112.1}; 
    double R3_unc_Counts[2] = {305.67, 89.76}; 

    double R1_Eff[2] = {0.67848, 0.0834212}; 
    double R2_Eff[2] = {0.593998, 0.0792668}; 
    double R3_Eff[2] = {0.625868, 0.0664421}; 

    double B235 = 57.2;
    double B238 = 0.842;

    double decay235 = 9.85452e-10;
    double decay238 = 1.55101e-10;

    double R_1 = 0.00276725;
    double R_2 = 0.00282513;
    double R_3 = 0.00241883;

    // Calculate and print uncertainties for each run
    cout << "The Run1 Uncertainty of ratio:  " 
         << Unc_Calc(R1_unc_Counts[0], R1_unc_Counts[1], R1_Eff[0], R1_Eff[1], B235, B238, decay235, decay238, R_1, R1_Count_atm[0], R1_Count_atm[1]) << endl; 

    cout << "The Run2 Uncertainty of ratio:  " 
         << Unc_Calc(R2_unc_Counts[0], R2_unc_Counts[1], R2_Eff[0], R2_Eff[1], B235, B238, decay235, decay238, R_2, R2_Count_atm[0], R2_Count_atm[1]) << endl; 

    cout << "The Run3 Uncertainty of ratio:  " 
         << Unc_Calc(R3_unc_Counts[0], R3_unc_Counts[1], R3_Eff[0], R3_Eff[1], B235, B238, decay235, decay238, R_3, R3_Count_atm[0], R3_Count_atm[1]) << endl; 
}


















