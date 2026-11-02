
/* 


Q_6.  Using the calibration determined in question (2) and peak parameters from question (5), determine the energies and errors for your europium peaks. Match your peaks to known gamma rays produced in the decay of Eu-152. Find the energy residuals (Residual = Fit_energy - Literature_energy) and errors.


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
Double_t Energies[9] = {1173.237, 1332.501, 511.006, 1274.53, 661.657, 121.294, 244.271, 780.112, 1407.89};
Double_t Channel_N[9]  = {3531.45, 4016.93, 1511.93, 3840.33, 1972.05, 323.96, 698.94, 2332.82, 4247.03};
Double_t Channel_err[9] = {0.12, 0.13, 0.06, 0.11, 0.06, 0.021, 0.094, 0.11, 0.105};
Double_t Energies_err[9] = {0.004, 0.005, 0.001, 0.02, 0.003, 0.03, 0.02, 0.012, 0.03};
//Number of entries for each
int n = 9;

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
double Cov_ab = -1.568e-07;



//Printing the Calibration error Formula and values
vector <double> Calibration_error(9); 
// sigma_f(x)^2 = x^2*sigma_a^2 + sigma_b^2 + 2*x*cov(a,b) refrence formula of error of fit
   for(int j = 0; j<9; j++){
        Calibration_error[j] = sqrt(sigma_a*sigma_a*Channel_N[j]*Channel_N[j] + sigma_b*sigma_b + 2*Channel_N[j]*Cov_ab);
        
        cout <<"the channel number:" << Channel_N[j] <<"  The Energy error: Sigma_err = " << Calibration_error[j] << endl;  
           
   }
    
        

//Printing the Energy Calibration formula
vector <double> New_Energies(9);
for(int i=0; i<9; i++) {

New_Energies[i] = (a*Channel_N[i] + b); 

cout << "the channel number:" << Channel_N[i] << "  The gamma energy: E_gamma = " <<  New_Energies[i] << endl;

}



// Lit Values and errors 
double Energy_gl[4] = {121.782, 244.697, 778.904, 1408.006}; 
double Energy_err_gl[4] {0.001, 0.001, 0.002, 0.003}; 


//Residual and errors of Eu Peaks: 
    double R_1 =  (New_Energies[5] - Energy_gl[0]); 
    double Sigma_R1 = sqrt(pow(Energy_err_gl[0],2)  + pow(Calibration_error[5],2)); 
    cout << "The Residual is: R = " << R_1 << " Corresponding error is: Sigma_R = " << Sigma_R1 <<endl; 
    
    double R_2 =  (New_Energies[6] - Energy_gl[1]); 
    double Sigma_R2 = sqrt(pow(Energy_err_gl[1],2)  + pow(Calibration_error[6],2)); 
    cout << "The Residual is: R = " << R_2 << " Corresponding error is: Sigma_R = " << Sigma_R2 <<endl; 
    
    
    double R_3 =  (New_Energies[7] - Energy_gl[2]); 
    double Sigma_R3 = sqrt(pow(Energy_err_gl[2],2)  + pow(Calibration_error[7],2)); 
    cout << "The Residual is: R = " << R_3 << " Corresponding error is: Sigma_R = " << Sigma_R3 <<endl; 
    
    
    double R_4 =  (New_Energies[8] - Energy_gl[3]); 
    double Sigma_R4 = sqrt(pow(Energy_err_gl[3],2)  + pow(Calibration_error[8],2)); 
    cout << "The Residual is: R = " << R_4 << " Corresponding error is: Sigma_R = " << Sigma_R4 <<endl; 
    
    
// Residuals and their corresponding errors
Double_t Residuals[4] = {-0.487972, -0.425874, 1.20856, -0.115989};
Double_t ResidualErrors[4] = {0.0181455, 0.0155289, 0.0106056, 0.0220278}; 
    
    
 
 
 
// Create a TLegend to display the Fit and Energy information
TLegend *legend = new TLegend(0.1, 0.7, 0.48, 0.9); // Adjust the position and size
legend->SetTextSize(0.02); // Small size to fit   



// Add the Energy values and their errors to the legend
    for (int i = 5; i < 9; ++i) {
        legend->AddEntry((TObject*)0, Form("Fit Energy: %.3f +/- %.3f keV", New_Energies[i], Calibration_error[i]), "");
    }
    
    
// Add the Literature values and their errors to the legend
    for (int j = 0; j < 4; ++j) {
        legend->AddEntry((TObject*)0, Form("Lit Energy: %.3f +/- %.3f keV", Energy_gl[j], Energy_err_gl[j]), "");
    }
    
    
    
// Add the Residuals and their errors to the legend
    for (int k = 0; k < 4; ++k) {
       legend->AddEntry((TObject*)0, Form("Residual: %.3f +/- %.3f", Residuals[k], ResidualErrors[k]), "");
    }
    
    
    
    
    
    
    
    
    
    
    

// Draw the legend on the canvas
legend->Draw();
// Update the canvas to display the results
c1->Update();
    
    
    
    
    
     
    
    
    
    
}

void Run1() {
    Energy_Calibration();
}




