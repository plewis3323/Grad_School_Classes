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
#include <TMath.h>
#include <vector>
#include <string>
#include <TLegend.h>
#include <TLatex.h>
#include <cmath>
using namespace std;

void getUserDefinedRange(double &start, double &end, const string &prompt) {
    cout << prompt << endl;
    cout << "Start: ";
    cin >> start;
    cout << "End: ";
    cin >> end;
}

TF1* fitPeak(TH1F *myhist, int peakNumber, vector<string> &descriptions) {
    double fitStart, fitEnd, bgStart1, bgEnd1, bgStart2, bgEnd2;

    // Prompt user for ranges
    getUserDefinedRange(fitStart, fitEnd, "Enter fit range for Peak " + to_string(peakNumber) + ":");
    /*
    getUserDefinedRange(bgStart1, bgEnd1, "Enter first background range for Peak " + to_string(peakNumber) + ":");
    getUserDefinedRange(bgStart2, bgEnd2, "Enter second background range for Peak " + to_string(peakNumber) + ":");
    */

    // Fit the peak
    TString funcName = TString::Format("gausFit%d", peakNumber);
    TF1 *gausFit = new TF1(funcName, "gaus", fitStart, fitEnd);
    myhist->Fit(gausFit, "R");
    gausFit->SetLineColor(peakNumber + 2); // Assign different colors for each peak
    gausFit->Draw("SAME");

    // Output fit results
    cout << "Peak " << peakNumber << " Mean = " << gausFit->GetParameter(1) << endl;
    double N_t = gausFit->Integral(fitStart, fitEnd);
    cout << "Peak " << peakNumber << " Total: N_t = " << N_t << endl;

   /* double N_b = (myhist->Integral(bgStart1, bgEnd1) + myhist->Integral(bgStart2, bgEnd2)) / 2;
    cout << "Peak " << peakNumber << " Background: N_b = " << N_b << endl;
   */
   
    double N_b = 0.0; 
   
    double N_s = (N_t - N_b);
    cout << "Peak " << peakNumber << " Signal: N_s = " << N_s << endl;

    double std_dev = gausFit->GetParameter(2);
    cout << "Peak " << peakNumber << " Std Error = " << std_dev / sqrt(N_s) << endl;
    
    double Count_err = sqrt(N_t + N_b); 
    cout << "Peak " << peakNumber << " Sigma_counts = " << Count_err << endl; 
   

    // Add description for the legend
    descriptions.push_back("Mean Channel = " + to_string(gausFit->GetParameter(1)));

    return gausFit;
}

void addLegendToCanvas(vector<TF1*> fits, vector<string> descriptions, TCanvas *canvas) {
    if (fits.size() != descriptions.size()) {
        cerr << "Error: The number of fits and descriptions must be the same." << endl;
        return;
    }

    // Create a new legend
    TLegend *legend = new TLegend(0.35, 0.4, 0.65, 0.6); // Adjust position if needed
    legend->SetTextSize(0.03); // Adjust text size
    legend->SetBorderSize(0); // No border
    legend->SetFillStyle(0);  // Transparent background

    // Add each fit and description to the legend
    for (size_t i = 0; i < fits.size(); ++i) {
        legend->AddEntry(fits[i], descriptions[i].c_str(), "l");
    }

    // Draw the legend
    canvas->cd();
    legend->Draw();
}

void Sources(TString directory, TString filename, TString histname, TCanvas *Sources1) {
    Int_t bin, data;
    ifstream infile;

    TString filepath = directory + "/" + filename; // Construct the full file path
    infile.open(filepath);
    if (!infile.good()) {
        cerr << "Couldn't open file " << filepath << "." << endl;
        infile.close();
        return;
    }

    Int_t minimum = kMaxInt;
    Int_t maximum = kMinInt;

    while (!infile.eof()) {
        infile >> bin >> data;
        if (bin > maximum)
            maximum = bin;
        if (bin < minimum)
            minimum = bin;
    }

    if (minimum > maximum) {
        cerr << "Minimum is greater than Maximum: " << minimum << " " << maximum << endl;
        return;
    }

    TH1F *myhist = new TH1F(histname, histname, maximum - minimum + 1, minimum - .5, maximum + .5);

    infile.clear();
    infile.seekg(0, ios::beg);

    while (!infile.eof()) {
        infile >> bin >> data;
        for (size_t i = 0; i < data; i++) {
            myhist->Fill(bin);
        }
    }

    infile.close();

    Sources1->cd();
    myhist->Draw();
    myhist->GetXaxis()->SetRangeUser(0, 6000);

    // Fit and analyze peaks
    vector<TF1*> fits;
    vector<string> descriptions;

    fits.push_back(fitPeak(myhist, 1, descriptions));
    fits.push_back(fitPeak(myhist, 2, descriptions));
    fits.push_back(fitPeak(myhist, 3, descriptions));

    // Add legend
    addLegendToCanvas(fits, descriptions, Sources1);

    // Customize axes
    myhist->GetXaxis()->SetTitle("Channel Number (Arb. Unit)");
    myhist->GetYaxis()->SetTitle("Counts (Arb. Unit)");

    gStyle->SetOptStat(0);
}




void Run1() {
    TString directory = "/home/plewis/Gradlab/Lab_reports/Lab4/data_f/Nov_6_EnV"; 
    TCanvas *c1 = new TCanvas("c1");
    Sources(directory, "tag1061.xy", "Counts vs. Channel Number", c1);
    TFile *Histo_data = new TFile("O_tag1061C.root", "RECREATE");
    c1->Write();
    Histo_data->Close();
}



/* ----------------------------------------------------------------------------------------------------- */



void Raw_Sources(TString directory, TString filename, TString histname, TCanvas* Sources1)

{



    Int_t bin, data;
    ifstream infile;

    TString filepath = directory + "/" + filename; // Construct the full file path
    infile.open(filepath);
    if (!infile.good()) {
        cerr << "Couldn't open file " << filepath << "." << endl;
        infile.close();
        return;
    }

    Int_t minimum = kMaxInt;
    Int_t maximum = kMinInt;

    while (!infile.eof()) {
        infile >> bin >> data;
        if (bin > maximum)
            maximum = bin;
        if (bin < minimum)
            minimum = bin;
    }

    cout << "after first loop" << endl;

    if (minimum > maximum) {
        cerr << "Minimum is greater than Maximum: " << minimum << " " << maximum << endl;
        return;
    }
    cout << "after test on minimum>maximum" << endl;

    TH1F *myhist = new TH1F(histname, histname, maximum - minimum + 1, minimum - .5, maximum + .5);

    cout << "after making the hist" << endl;

    infile.clear();
    infile.seekg(0, ios::beg);

    while (!infile.eof()) {
        infile >> bin >> data;
        if (bin % 100 == 0)
            cout << "\t" << bin << "\t" << data << endl;
        for (size_t i = 0; i < data; i++) {
            myhist->Fill(bin);
        }
    }

    infile.close();

    Sources1->cd();
    // Drawing the histogram on the canvas
    myhist->Draw();

    // Zoom in on the x-axis
    myhist->GetXaxis()->SetRangeUser(0, 6000);


    
}

void Run2()

{



    TString directory = "/home/plewis/Gradlab/Lab_reports/Lab4/data_f/Nov_6_EnV"; // Update this with the desired directory path
    TCanvas *c2 = new TCanvas("c2");
    Raw_Sources(directory, "tag1061.xy", "Counts vs. Channel Number", c2);
    TFile *Histo_data = new TFile("R_tag1061.root", "RECREATE");
    c2->Write();
    Histo_data->Close();
    
    
}






















