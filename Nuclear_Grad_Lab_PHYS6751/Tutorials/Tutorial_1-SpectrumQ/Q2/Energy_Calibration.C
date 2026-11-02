


/* 


Q_2.  Determine the energy calibration using the five peaks in the Na-22, Co-60, and Cs-137 spectra.  Report the covariance matrix and the formula for the calibration error.



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


void Energy_Calibration () {


// Known energies in (keV) and corresponding channel numbers and std deviations
Double_t Energies[5] = {1173.237, 1332.501, 511.006, 1274.53, 661.657};
Double_t Channel_N[5]  = {3531.45, 4016.93, 1511.93, 3840.33, 1972.05};
Double_t Channel_err[5] = {0.12, 0.13, 0.06, 0.11, 0.06};
Double_t Energies_err[5] = {0.004, 0.005, 0.001, 0.02, 0.003};
//Number of entries for each
int n = 5;


// Create TGraphErrors
TGraphErrors* graph = new TGraphErrors(n, Channel_N, Energies, Channel_err, Energies_err);


// Fit a linear function
TF1 *Calibration = new TF1("Calibration", "[0]*x + [1]", 0, 8000); 
graph->Fit(Calibration);



// Draw the graph and save the output
TCanvas *c1 = new TCanvas("c1", "Energy Calibration", 800, 600);
graph->SetTitle("Energy Calibration ");
graph->Draw("AP");
graph->SetMarkerStyle(8);
c1->SaveAs("energy_calibration.pdf");
graph->SetMarkerStyle(21);  // Set a specific marker style
graph->SetMarkerSize(1.0);  // Set marker size
graph->SetMarkerColor(kBlue);  // Set marker color
graph->SetLineColor(kRed);  // Set line color for error bars
graph->SetLineWidth(5);     // Set line width for error bars


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


// Print the covariance matrix in normal form
cout << "\nCovariance Matrix:\n";


//printing covarience elements in a matrix: 

//string matrix to be printed to the console
cout << "[[ sigma_a^2   Cov(a,b) ]" << endl;
cout << " [ Cov(b,a)    sigma_b^2 ]]" << endl;

   
cov.Print();
double Cov_ab = -3.888e-07;



//Printing the Calibration error Formula and values
vector <double> Calibration_error(5); 
// sigma_f(x)^2 = x^2*sigma_a^2 + sigma_b^2 + 2*x*cov(a,b) refrence formula of error of fit

   for(int j = 0; j<5; j++){
        Calibration_error[j] = sqrt(sigma_a*sigma_a*Channel_N[j]*Channel_N[j] + sigma_b*sigma_b + 2*Channel_N[j]*Cov_ab);
        cout <<"\nThe Calibration error of channel:\n" <<Channel_N[j] << "\nis\n" << Calibration_error[j];
        }
        cout << endl;
        
        

//Printing the Energy Calibration formula
vector <double> New_Energies(5);

for(int i=0; i<5; i++) {

New_Energies[i] = (a*Channel_N[i] + b); 
cout << "The New Energy values are from Channel:  " << Channel_N[i] << "\nare\n" << New_Energies[i] << endl;

}

}


void Run1() {
    Energy_Calibration();
}




















