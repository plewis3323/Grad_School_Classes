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
#include <string>
#include <TLegend.h>
#include <cmath>
#include <TMath.h>
#include <TLatex.h>
using namespace std; 



void efficiency_calculation(TCanvas *Eff) {

    Eff->cd();
    Eff->SetGrid();

    // Number of peaks outside the reference
    const int nPeaks = 8;
    double N_ref = 43399.7; 
    double N_i[nPeaks] = {6363.96, 15362.8, 667.661, 845.467, 1928.36, 1052.93, 1358.98, 2183.7};  
    double sigma_N_i[nPeaks] = {100.319, 134.584, 57.608, 67.15, 58.15, 51.22, 55.68, 49.48};  // uncertainties in N_i
    double sigma_N_ref = 227.815;   // uncertainty in N_ref
    // Arrays to store calculated efficiencies and errors
    double efficiency[nPeaks];
    double sigma_efficiency[nPeaks];
    double energy[nPeaks] = {244.697, 344.279, 443.965, 778.821, 964.079, 1085.869, 1112.069, 1408.006};  // energies
    double energy_err[nPeaks] = {0.001, 0.001, 0.003, 0.002, 0.018, 0.024, 0.003, 0.003};  // uncertainty in energies
   double I_ref = 28.37; 
   double I_i[nPeaks] = {7.53, 26.57, 3.125, 12.97, 14.63, 10.13,  13.54, 20.85};  // intensities of other peaks
   double sigma_I_i[nPeaks] = {0.04, 0.11, 0.014, 0.06, 0.06, 0.05, 0.06, 0.09};  // uncertainties in I_i
   double sigma_I_ref = 0.13;  // uncertainty in I_ref
           

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
    TGraphErrors *graph = new TGraphErrors(nPeaks, energy, efficiency, energy_err, sigma_efficiency);
    graph->SetTitle("Run3: Relative Efficiency vs Energy;Energy (keV);Relative Efficiency");
    graph->SetMarkerStyle(21);
    graph->SetMarkerColor(kBlue);
    graph->Draw("AP");

    // Fit the graph with a quadratic function for low energies
    TF1 *fitFunc1 = new TF1("fitFunc1", " [0]*x^2  + [1]*x + [2]", 0, 500);  
    fitFunc1->SetLineColor(kRed);
    graph->Fit(fitFunc1, "R");

    // Fit the graph with a linear function for higher energies
    TF1 *fitFunc2 = new TF1("fitFunc2", "[0]*x + [1]", 750, 1500);  
    fitFunc2->SetLineColor(kGreen);
    graph->Fit(fitFunc2, "R+");

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph, "Data points", "p");
    legend->AddEntry(fitFunc1, "Pol2 Fit (0-500 keV)", "l");
    legend->AddEntry(fitFunc2, "Linear Fit (750-1500 keV)", "l");
    legend->Draw();
}




void Run1() { 



TCanvas *c1 = new TCanvas("c1", "Efficiency vs Energy", 800, 600);
efficiency_calculation(c1); 
TFile *Eff1 = new TFile("Efficiencies_3rd_run.root", "RECREATE"); 
c1->Write();
Eff1->Close(); 


}  




































































































































































































































