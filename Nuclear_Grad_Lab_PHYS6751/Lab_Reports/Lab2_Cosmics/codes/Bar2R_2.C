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
    Double_t Voltages[14] = {50.9, 58.1, 71.7, 81.9, 90.7, 100.9, 110.5, 121.4, 130.7, 
                             141.6, 150.4, 20.8, 30.2, 40.0};
    Double_t Counts[14] = {14744, 13962, 12150, 12212, 10323, 9210, 7756, 6433, 5579, 
                           4909, 4612, 18627, 17420, 15977};

    vector<double> C_rates(14); 
    for(int i = 0; i < 14; i++) {
        C_rates[i] = Counts[i] / t; 
        cout << "The Count rates are: " << C_rates[i] << endl; 
    }

    vector<double> CountR_errors(14);
    for(int j = 0; j < 14; j++) {
        CountR_errors[j] = sqrt(C_rates[j]); 
        cout << "The Counts are: " << Counts[j] << endl;
        cout << "The Voltages are: " << Voltages[j] << endl;
        cout << "The Count errors are: " << CountR_errors[j] << endl; 
    }

    int n = 14;

    // Create TGraphErrors for data points
    TGraphErrors* graph = new TGraphErrors(n, Voltages, C_rates.data(), 0, CountR_errors.data());
    graph->SetTitle("Bar#2 Right: Threshold");
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(2.0);  
    graph->SetMarkerColor(kRed);  
    graph->SetLineWidth(2);     
    graph->Draw("AP");

    // Fit the graph with a cubic function
    TF1 *fitFunc1 = new TF1("fitFunc1", "pol3", 0, 200);  
    fitFunc1->SetLineColor(5);
    graph->Fit(fitFunc1, "R");

    // Draw a vertical line at 76.8 mV (threshold voltage)
    TLine *thresholdLine = new TLine(76.8, 0, 76.8, *max_element(C_rates.begin(), C_rates.end()));
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
    legend->AddEntry(thresholdLine, "Threshold Voltage at 76.8 mV", "l");
    legend->Draw();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1"); 
    Threshold(c1);
    TFile *Bars = new TFile("Bar2R.root", "RECREATE"); 
    c1->Write();
    Bars->Close(); 
}




