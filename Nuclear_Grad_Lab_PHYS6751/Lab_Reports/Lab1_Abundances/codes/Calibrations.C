

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


void Energy_Calibration (TCanvas* ECalibration) {





ECalibration->cd(); 

// Known energies in (keV) and corresponding channel numbers and std deviations
Double_t Energies[7] = {276.400, 302.851, 356.013, 383.848, 1173.237, 1332.501, 661.657};
Double_t Channel_N[7]  = {1563.71, 1700.76, 1976.12, 2120.32, 6202.74, 7025.74, 3556.72};
Double_t Channel_err[7] = {0.122, 0.069, 0.0422, 0.122, 0.123, 0.148, 0.077};
Double_t Energies_err[7] = {0.001, 0.001, 0.001, 0.001, 0.004, 0.005, 0.003};
//Number of entries for each
int n = 7;


// Create TGraphErrors
TGraphErrors* graph = new TGraphErrors(n, Channel_N, Energies, Channel_err, Energies_err);


// Fit a linear function
TF1 *Calibration = new TF1("Calibration", "[0]*x + [1]", 0, 8000); 
graph->Fit(Calibration);



graph->SetTitle("Energy Calibration ");
graph->Draw("AP");
graph->SetMarkerStyle(8);
graph->SetMarkerStyle(21);  // Set a specific marker style
graph->SetMarkerSize(1.0);  // Set marker size
graph->SetMarkerColor(kBlue);  // Set marker color
graph->SetLineColor(15);  // Set line color for error bars
graph->SetLineWidth(7);     // Set line width for error bars


// Center the X-axis label by adjusting the offset and title size
graph->GetXaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetXaxis()->SetTitleSize(0.05);  // Adjust the size if necessary

// Center the Y-axis label by adjusting the offset and title size
graph->GetYaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetYaxis()->SetTitleSize(0.05);  // Adjust the size if necessary


// Set the X-axis and Y-axis labels
graph->GetXaxis()->SetTitle("Channel Number");
graph->GetYaxis()->SetTitle("E (kev)");


// Obtain parameters 
double a = Calibration->GetParameter(0);
double b = Calibration->GetParameter(1);



//Calculate and print the error in the calibration
double sigma_a = Calibration->GetParError(0);
double sigma_b = Calibration->GetParError(1);
cout << "Slope (a): " << a << " ± " << sigma_a << endl;
cout << "Intercept (b): " << b << " ± " << sigma_b << endl;

// Print the calibration formula 
cout << "Calibration: E = " << a << " * channel + " << b << endl;





// Perform the fit and store the result
TFitResultPtr result = graph->Fit(Calibration, "S");  // "S" option stores the result
// Get the covariance matrix from the fit result
TMatrixDSym cov = result->GetCovarianceMatrix();
cov.Print();
double Cov_ab = -4.118e-08; 



//Printing the Energy Calibration formula
vector <double> New_Energies(7);
for(int i=0; i<7; i++) {

New_Energies[i] = (a*Channel_N[i] + b); 
cout << "The New Energy values are from Channel:  " << Channel_N[i] << "\nare\n" << New_Energies[i] << endl;

}



//Printing the Calibration error Formula and values
vector <double> Calibration_error(7); 
// sigma_f(x)^2 = x^2*sigma_a^2 + sigma_b^2 + 2*x*cov(a,b) refrence formula of error of fit
   for(int j = 0; j<7; j++){
        Calibration_error[j] = sqrt(sigma_a*sigma_a*Channel_N[j]*Channel_N[j] + sigma_b*sigma_b + 2*Channel_N[j]*Cov_ab);
        cout <<"\nThe Calibration error of channel:\n" <<Channel_N[j] << "\nis\n" << Calibration_error[j];
        }
        cout << endl;
        


}



void Run1() {



TCanvas *c1 = new TCanvas("c1"); 
Energy_Calibration(c1);
TFile *Histo_data = new TFile("Calibrations.root", "RECREATE"); 
c1->Write();
Histo_data->Close(); 


    
}


















