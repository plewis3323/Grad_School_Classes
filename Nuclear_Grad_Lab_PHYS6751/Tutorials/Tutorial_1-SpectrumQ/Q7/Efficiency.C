/* 


Q_7. Using your europium peaks, determine the relative detector efficiency as a function of energy. This can be a piecewise-linear function, or you can choose a more complicated form.  Include error analysis.


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

void efficiency_calculation() {
    //Number of peaks outside the refrence 
    const int nPeaks = 3;
    double N_ref = 23428.0; // counts Peak #1: 310-340 bin range
    double I_ref = 100.0;  // intensity of the reference peak
    double N_i[nPeaks] = {1895.0, 1648.0, 1676.0};  // counts in other peaks
    double I_i[nPeaks] = {27.9, 48.0, 78.10};  // intensities of other peaks
    double sigma_N_i[nPeaks] = {99.47, 56.10, 40.94 };  // uncertainties in N_i
    double sigma_I_i[nPeaks] = {0.03, 0.09, 0.07};  // uncertainties in I_i
    double sigma_N_ref = 196.03 ;  // uncertainty in N_ref
    double sigma_I_ref = 0.12;  // uncertainty in I_ref

    // Arrays to store calculated efficiencies and errors
    double efficiency[nPeaks];
    double sigma_efficiency[nPeaks];
    double energy[nPeaks] = {244.697, 778.904, 1408.006};  //energies
    double energy_err[nPeaks] = {0.001, 0.002, 0.003};  //uncertainty in energies

    // Calculate relative efficiencies and their uncertainties
    for (int i = 0; i < nPeaks; ++i) {
        double expected_i = (N_ref / I_ref) * I_i[i];
        efficiency[i] = (N_i[i] * I_ref) / (N_ref * I_i[i]);

        // Error propagation
        double fractional_uncertainty = pow(sigma_N_i[i] / N_i[i], 2)
                                      + pow(sigma_I_ref / I_ref, 2)
                                      + pow(sigma_N_ref / N_ref, 2)
                                      + pow(sigma_I_i[i] / I_i[i], 2);
        sigma_efficiency[i] = efficiency[i] * sqrt(fractional_uncertainty);

        std::cout << "Efficiency for peak " << i+2 << " (Energy = " << energy[i] << " keV): "
                  << efficiency[i] << " ± " << sigma_efficiency[i] << std::endl;
    }

    // Create a graph of efficiency vs energy with error bars
    TCanvas *c1 = new TCanvas("c1", "Efficiency vs Energy", 800, 600);
    TGraphErrors *graph = new TGraphErrors(nPeaks, energy, efficiency, energy_err, sigma_efficiency);
    graph->SetTitle("Relative Efficiency vs Energy;Energy (keV);Relative Efficiency");
    graph->SetMarkerStyle(21);
    graph->SetMarkerColor(kBlue);
    graph->Draw("AP");

    // Optional: Fit the graph with a special function
    TF1 *fitFunc = new TF1("fitFunc", "[0] / pow(x, [1])", 0, 1200);  // Exp fit as an example
    graph->Fit(fitFunc, "R");
    c1->Update();
    cout << "Relative Efficieny relation: R_rel =  7.4/(x^0.59)" << endl; 
    
    
    
    
    
    
    
     // Create a legend to display all information
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);  // Set the position (x1, y1, x2, y2)
    legend->SetHeader("Relative Efficiencies and Data", "C");  // Add a header
    legend->SetTextSize(0.03);

    // Add the reference efficiency, intensity, and error to the legend
    legend->AddEntry((TObject*)0, "Reference Efficiency = 1.0", "");
    legend->AddEntry((TObject*)0, Form("Ref Intensity = %.2f +/- %.2f", I_ref, sigma_I_ref), "");
    legend->AddEntry((TObject*)0, Form("Ref Eff Error = 0.001 "), "");

    // Loop through the points and add each to the legend
    for (int i = 0; i < nPeaks; ++i) {
        legend->AddEntry((TObject*)0, Form("Eff_%d = %.2f +/- %.2f", i + 2, efficiency[i],  sigma_efficiency[i]), "");
        legend->AddEntry((TObject*)0, Form("Int_%d = %.2f +/- %.2f", i + 2, I_i[i], sigma_I_i[i]), "");
    }
    
    // Add the relative efficiency relation to the legend
    legend->AddEntry((TObject*)0, "R_rel = 7.4 / (x^0.59)", "");

    // Draw the legend
    legend->Draw();

    // Update the canvas to display everything
    c1->Update();
    
    
    
    
    
    
    
    
    
    
    
}






void Run1() {


efficiency_calculation();


}





































































































































































