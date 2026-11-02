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
#include <cmath>
using namespace std;

void Theta_Dist(TCanvas* tCalibration) {
    // Number of entries for each
    int n = 10;
    tCalibration->cd(); 
    Double_t t = 30.0; 
    Double_t Zen_C = 90.0; 
    Double_t theta[10] = {Zen_C - 90.0, Zen_C - 80.0, Zen_C - 70.0, Zen_C - 60.0, Zen_C - 50.0, Zen_C - 40.0, Zen_C - 30.0, Zen_C - 20.0, Zen_C - 10.0, Zen_C - 0.0};
    Double_t Rate[10]  = {106.0/t, 135.0/t, 115.0/t, 87.0/t, 59.0/t, 53.0/t, 33.0/t, 17.0/t, 12.0/t, 11.0/t};
    vector <Double_t> Rate_Errors(10); 

    for(int i = 0; i < n;  i++) {
        Rate_Errors[i] = sqrt(Rate[i]/t);
        cout << "The Rate Errors are: " << Rate_Errors[i] << endl; 
    } 

    vector <Double_t> Rad(10); 
    for(int j = 0; j < n;  j++) {
        Rad[j] = theta[j] * (TMath::Pi()/180);
        cout << "The angles in radians are: " << Rad[j] << endl; 
    } 

    vector <Double_t> Cos_Square(10); 
    for(int k = 0; k < n;  k++) {
        Cos_Square[k] = TMath::Cos(theta[k] * (TMath::Pi()/180)) * TMath::Cos(theta[k] * (TMath::Pi()/180));
        cout << "The cos^2 values are: " << Cos_Square[k] << endl; 
    } 

    // Create TGraphErrors
    TGraphErrors* graph = new TGraphErrors(n, Rad.data(), Rate, 0, Rate_Errors.data());
    // Fit a linear function
     TF1 *Calibration = new TF1("Calibration", "[0]*cos([1]*x + [2])*cos([1]*x + [2]) + [3]", 0, 1.8); 
     Calibration->SetParameters(10, 1, 0, 10);
    graph->Fit(Calibration);
    graph->SetTitle("Count Rate vs. Angle");
    graph->Draw("AP");
    graph->SetMarkerStyle(21);  
    graph->SetMarkerSize(1.0);  
    graph->SetMarkerColor(kBlue);  
    graph->SetLineColor(15);  
    graph->SetLineWidth(7);     

    graph->GetXaxis()->SetTitleOffset(0.95); 
    graph->GetXaxis()->SetTitleSize(0.05);  
    graph->GetYaxis()->SetTitleOffset(0.95); 
    graph->GetYaxis()->SetTitleSize(0.05);  

    graph->GetXaxis()->SetTitle("Zenith Angle (rad)");
    graph->GetYaxis()->SetTitle("Rate (Counts/Sec)");

    TLegend *legend = new TLegend(0.1, 0.7, 0.4, 0.9);
    legend->AddEntry(graph, "Exp Rates", "p");
    legend->AddEntry(Calibration, "f(x) = Acos^{2}(#omega x + #phi) +B", "l");
    legend->Draw();
}

void Theta_Dist2(TCanvas* tCalibration) {
    // Number of entries for each
    int n = 10;
    tCalibration->cd(); 
    Double_t t = 30.0; 
    Double_t Zen_C = 90.0;
  Double_t theta[10] = {Zen_C-90.0, Zen_C - 80.0, Zen_C - 70.0, Zen_C - 60.0, Zen_C - 50.0, Zen_C - 40.0, Zen_C - 30.0, Zen_C - 20.0, Zen_C - 10.0, Zen_C -0.0};
    Double_t Rate[10]  = {106.0/t, 135.0/t, 115.0/t, 87.0/t, 59.0/t, 53.0/t, 33.0/t, 17.0/t, 12.0/t, 11.0/t};
    vector <Double_t> Rate_Errors(10); 

    for(int i = 0; i < n;  i++) {
        Rate_Errors[i] = sqrt(Rate[i]/t);
        cout << "The Rate Errors are: " << Rate_Errors[i] << endl; 
    } 
    
    for(int l = 0; l < n;  l++) {
        cout <<"the Count Rates are: " << Rate[l] << endl; 
    } 


    vector <Double_t> Rad(10); 
    for(int j = 0; j < n;  j++) {
        Rad[j] = theta[j] * (TMath::Pi()/180);
        cout << "The angles in radians are: " << Rad[j] << endl; 
    } 

    vector <Double_t> Cos_Square(10); 
    for(int k = 0; k < n;  k++) {
        Cos_Square[k] = TMath::Cos(theta[k] * (TMath::Pi()/180)) * TMath::Cos(theta[k] * (TMath::Pi()/180));
        cout << "The cos^2 values are: " << Cos_Square[k] << endl; 
    } 
    
    Double_t a = 3.57; 
    Double_t b = 0.27;
    vector <Double_t> Cal_CountR(10); 
    for(int h = 0; h < n; h++) 
    {
         Cal_CountR[h] = (Cos_Square[h]*a + b); 
         cout << "The Calculated Count Rates:  " << Cal_CountR[h] << endl; 
         
    }
    
    vector <Double_t> Residuals(10); 
    vector <Double_t> Residual_Err(10); 
    for(int f = 0; f < n; f++) 
    { 
    
        Residuals[f] = (Rate[f] - Cal_CountR[f]); 
        Residual_Err[f] = sqrt(Rate[f] + Cal_CountR[f]); 
        cout << " Residuals are:  " << Residuals[f] << " +/- "  << Residual_Err[f] << endl; 
    
    }
    
    
    // Create TGraphErrors
    TGraphErrors* graph2 = new TGraphErrors(n, Cos_Square.data(), Rate, 0, Rate_Errors.data());
    // Fit a linear function
    TF1 *Calibration2 = new TF1("Calibration2", "[0]*x + [1]", 0, 10000); 
    graph2->Fit(Calibration2);

    graph2->SetTitle("Count Rate vs. cos^{2}(#theta)");
    graph2->Draw("AP");
    graph2->SetMarkerStyle(21);  
    graph2->SetMarkerSize(1.0);  
    graph2->SetMarkerColor(kBlue);  
    graph2->SetLineColor(15);  
    graph2->SetLineWidth(7);     

    graph2->GetXaxis()->SetTitleOffset(0.95); 
    graph2->GetXaxis()->SetTitleSize(0.05);  
    graph2->GetYaxis()->SetTitleOffset(0.95); 
    graph2->GetYaxis()->SetTitleSize(0.05);  

    graph2->GetXaxis()->SetTitle("cos^{2}(#theta)");
    graph2->GetYaxis()->SetTitle("Rate (Counts/Sec)");

    // Add a legend to the canvas
    TLegend *legend2 = new TLegend(0.1, 0.7, 0.4, 0.9);
    legend2->AddEntry(graph2, "Squared Cosine Values", "p");
    legend2->AddEntry(Calibration2, "y = mx + b", "l");
    legend2->Draw();
}

void Run1() {
    TCanvas *c1 = new TCanvas("c1");
    Theta_Dist(c1);
    TFile *Histo_data = new TFile("Theta_Dist.root", "RECREATE");
    c1->Write();
    Histo_data->Close();
}

void Run2() {
    TCanvas *c1 = new TCanvas("c1");
    Theta_Dist2(c1);
    TFile *Histo_data = new TFile("Cos_square_Dist.root", "RECREATE");
    c1->Write();
    Histo_data->Close();
}













