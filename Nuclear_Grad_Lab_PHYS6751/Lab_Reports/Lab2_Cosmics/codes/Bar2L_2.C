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
    Double_t Voltages[18] = {50.2, 58.6, 69.5, 81.8, 91.5, 99.2, 111.3, 120.5, 132.6, 
                             140.2, 149.99, 171.8, 199.1, 230.2, 298.9, 22.1, 31.3, 40.1};
    Double_t Counts[18] = {11252, 11284, 10983, 11171, 11951, 10516, 10660, 10690, 10724, 
                           10814, 10255, 10067, 9644, 9288, 6579, 12414, 13082, 12138};

    vector<double> C_rates(18); 
    for(int i = 0; i < 18; i++) {
        C_rates[i] = Counts[i] / t; 
        cout << "The Count rates are: " << C_rates[i] << endl; 
    }

    vector<double> CountR_errors(18);
    for(int j = 0; j < 18; j++) {
        CountR_errors[j] = sqrt(C_rates[j]); 
        cout << "The Counts are: " << Counts[j] << endl;
        cout << "The Voltages are: " << Voltages[j] << endl;
        cout << "The Count errors are: " << CountR_errors[j] << endl; 
    }

    int n = 18;

    // Create TGraphErrors for data points
    TGraphErrors* graph = new TGraphErrors(n, Voltages, C_rates.data(), 0, CountR_errors.data());
    graph->SetTitle("Bar#2 Left: Threshold");
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(2.0);  
    graph->SetMarkerColor(kRed);  
    graph->SetLineWidth(2);     
    graph->Draw("AP");

    // Fit the graph with a cubic function
    TF1 *fitFunc1 = new TF1("fitFunc1", "pol3", 0, 300);  
    fitFunc1->SetLineColor(5);
    graph->Fit(fitFunc1, "R");

    // Draw a vertical line at 120.5 mV (threshold voltage)
    TLine *thresholdLine = new TLine(120.5, 0, 120.5, *max_element(C_rates.begin(), C_rates.end()));
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
    legend->AddEntry(fitFunc1, "Cubic Fit", "l");
    legend->AddEntry(thresholdLine, "Threshold Voltage at 120.5 mV", "l");
    legend->Draw();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    Threshold(c1);
    TFile *Bars = new TFile("Bar2L.root", "RECREATE"); 
    c1->Write();
    Bars->Close(); 
}





























































