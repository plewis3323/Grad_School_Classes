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
#include <TLegend.h>
#include <TLine.h>
#include <cmath>

using namespace std; 

void Threshold(TCanvas* Bar) {
    Bar->cd(); 
    int t = 20; // seconds

    // Number of entries for each
    Double_t Voltages[20] = {40.3, 45.6, 50.2, 57.2, 64.7, 71.8, 75.4, 80.2, 85.9, 90.1, 
                             97.5, 102.7, 108.1, 111.7, 152.2, 161.9, 170.0, 181.5, 190.1, 201.7};
    Double_t Counts[20] = {386887, 56657, 27569, 26479, 21823, 23969, 20679, 20030, 19302, 20092, 
                           19486, 19790, 21197, 19854, 18317, 17990, 17390, 17621, 17399, 15848};

    vector<double> C_rates(20);
    for(int i = 0; i < 20; i++) {
        C_rates[i] = Counts[i] / t; 
        cout << "The Count rates are: " << C_rates[i] << endl; 
    }

    vector<double> CountR_errors(20);
    for(int j = 0; j < 20; j++) {
        CountR_errors[j] = sqrt(C_rates[j]); 
        cout << "The Counts are: " << Counts[j] << endl;
        cout << "The Voltages are: " << Voltages[j] << endl;
        cout << "The Count errors are: " << CountR_errors[j] << endl; 
    }

    int n = 20;

    // Create TGraphErrors for data points
    TGraphErrors* graph = new TGraphErrors(n, Voltages, C_rates.data(), 0, CountR_errors.data());
    graph->SetTitle("Bar#1 Left: Threshold");
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(2.0);  
    graph->SetMarkerColor(kRed);  
    graph->SetLineWidth(2);     
    graph->Draw("AP");

    // Fit the graph with a cubic function
    TF1 *fitFunc1 = new TF1("fitFunc1", "pol3", 0, 230);  
    fitFunc1->SetLineColor(5);
    graph->Fit(fitFunc1, "R");

    // Draw a vertical line at 97.5 mV (threshold voltage)
    TLine *thresholdLine = new TLine(97.5, 0, 97.5, *max_element(C_rates.begin(), C_rates.end()));
    thresholdLine->SetLineColor(kGreen);
    thresholdLine->SetLineStyle(2);  // Dashed line
    thresholdLine->SetLineWidth(2);
    thresholdLine->Draw("SAME");

    // Adjust the axis labels
    graph->GetXaxis()->SetTitleOffset(0.95);
    graph->GetXaxis()->SetTitleSize(0.05);
    graph->GetYaxis()->SetTitleOffset(0.95);
    graph->GetYaxis()->SetTitleSize(0.05);

    // Set the X-axis and Y-axis labels
    graph->GetXaxis()->SetTitle("mV");
    graph->GetYaxis()->SetTitle("Counts/sec");

    // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph, "Data points", "p");
    legend->AddEntry(fitFunc1, "pol3 Fit", "l");
    legend->AddEntry(thresholdLine, "Threshold Voltage at 97.5 mV", "l");
    legend->Draw();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    Threshold(c1);
    TFile *Bars = new TFile("Bar1L.root", "RECREATE"); 
    c1->Write();
    Bars->Close(); 
}






