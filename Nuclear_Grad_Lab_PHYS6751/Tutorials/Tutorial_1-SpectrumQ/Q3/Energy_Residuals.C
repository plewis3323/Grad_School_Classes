

/* 


Q_3. Find the energy residuals (Residual = Fit_energy - Literature_energy) for your five calibration peaks. Comment on the size of the residuals compared to the calibration errors.



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
#include <TLegend.h>
#include <cmath>
using namespace std; 


void Energy_Residuals() {


// Known energies in (keV) and corresponding channel numbers
Double_t Lit_Energies[5] = {1173.237, 1332.501, 511.006, 1274.53, 661.657};
Double_t Fit_Energies[5] = {1173.21, 1332.43, 510.897, 1274.51, 661.796}; 
Double_t Lit_Energies_err[5] = {0.004, 0.005, 0.001, 0.02, 0.003};
Double_t Fit_Energies_err[5] = {0.0205, 0.0259, 0.0156, 0.0239, 0.0127};



//Co-60 For Peak #1: Energy Residual 
double CO_Res1 = Fit_Energies[0] - Lit_Energies[0]; 
cout << "The Cobalt Energy residual 1 =  " << CO_Res1 << endl; 




//Co-60 For Peak #2: Energy Residual 
double CO_Res2 = Fit_Energies[1] - Lit_Energies[1]; 
cout << "The Cobalt Energy residual 2 =  " << CO_Res2 << endl; 




//Na-22 For Peak #1 Energy Residual 
double Na_Res1 = Fit_Energies[2] - Lit_Energies[2]; 
cout << "The Sodium Energy residual 1 =  " <<  Na_Res1   << endl; 





// Na-22 For Peak #2 Energy Residual 
double Na_Res2 = Fit_Energies[3] - Lit_Energies[3]; 
cout << "The Sodium Energy residual 2 =  " <<  Na_Res2   << endl; 






// Cs-137 For Peak #1 Energy Residual
double Cs_Res1 = Fit_Energies[4] - Lit_Energies[4]; 
cout << "The Cesium Energy residual 1 =  " <<  Cs_Res1   << endl; 



//standard error of the residuals
vector<double> Res_error(5);
for(int i =0; i<5; i++) 
{

Res_error[i] = sqrt(Lit_Energies_err[i]*Lit_Energies_err[i] + Fit_Energies_err[i]*Fit_Energies_err[i]);
cout << "Residual_error #" << i << "  =  "<< Res_error[i] << endl;


}

}

void Run1() {
     Energy_Residuals(); 
}














