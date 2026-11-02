#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <vector>
using namespace std;

void AnalyzePeaks(TString directory, TString filename, TString outputRootFile) {
    // Open the input file
    TString filepath = directory + "/" + filename;
    ifstream infile(filepath);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file " << filepath << endl;
        return;
    }

    // Create histogram from input data
    Int_t bin, data;
    vector<pair<Int_t, Int_t>> data_points;
    while (infile >> bin >> data) {
        data_points.emplace_back(bin, data);
    }
    infile.close();

    // Find range for histogram
    Int_t minBin = data_points.front().first;
    Int_t maxBin = data_points.back().first;
    TH1F* hist = new TH1F("hist", "Histogram with Peaks", maxBin - minBin + 1, minBin, maxBin);

    // Fill histogram
    for (const auto& point : data_points) {
        hist->Fill(point.first, point.second);
    }

    // Create canvas for plotting
    TCanvas* canvas = new TCanvas("canvas", "Peak Analysis", 800, 600);
    hist->GetXaxis()->SetTitle("Channel Number (Arb. Unit)");
    hist->GetYaxis()->SetTitle("Counts (Arb. Unit)");
    hist->Draw();

    // Peak detection and fitting
    vector<TF1*> fittedPeaks;
    TLegend* legend = new TLegend(0.35, 0.4, 0.65, 0.6);
    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->SetHeader("Fitted Peaks");

    Int_t nBins = hist->GetNbinsX();
    for (Int_t i = 1; i < nBins - 1; ++i) {
        Double_t prev = hist->GetBinContent(i - 1);
        Double_t curr = hist->GetBinContent(i);
        Double_t next = hist->GetBinContent(i + 1);
        
        if (curr > prev && curr > next && curr - prev > 5 && curr - next > 5) { // Threshold for peak detection
            TString fitName = Form("Peak_%d", i);
            Double_t rangeLow = hist->GetBinLowEdge(i - 1);
            Double_t rangeHigh = hist->GetBinLowEdge(i + 1);

            TF1* peakFit = new TF1(fitName, "gaus", rangeLow, rangeHigh);
            hist->Fit(peakFit, "RQ"); // Quiet fit
            fittedPeaks.push_back(peakFit);

            // Set line color for visibility
            peakFit->SetLineColor(i + 2);
            peakFit->Draw("SAME");

            // Peak analysis and output
            Double_t mean = peakFit->GetParameter(1);
            legend->AddEntry(peakFit, Form("Peak: Mean Channel = %.3f", mean), "l");

            Double_t stdDev = peakFit->GetParameter(2);
            Double_t totalIntegral = peakFit->Integral(rangeLow, rangeHigh);

            Double_t background = (hist->Integral(hist->FindBin(rangeLow - stdDev), hist->FindBin(rangeLow)) +
                                   hist->Integral(hist->FindBin(rangeHigh), hist->FindBin(rangeHigh + stdDev))) / 2;
            Double_t signal = totalIntegral - background;

            cout << "=== Peak Analysis for " << fitName << " ===" << endl;
            cout << "Peak Mean = " << mean << endl;
            cout << "Peak Total (N_t) = " << totalIntegral << endl;
            cout << "Peak Background (N_b) = " << background << endl;
            cout << "Peak Signal (N_s) = " << signal << endl;
            cout << "Peak Std Error = " << stdDev / sqrt(signal) << endl;
        }
    }

    // Draw legend
    legend->Draw();

    // Save results to ROOT file
    TFile* outputFile = new TFile(outputRootFile, "RECREATE");
    hist->Write();
    for (TF1* fit : fittedPeaks) {
        fit->Write();
    }
    outputFile->Close();

    // Clean up
    delete canvas;
    delete hist;
    for (TF1* fit : fittedPeaks) {
        delete fit;
    }
}















