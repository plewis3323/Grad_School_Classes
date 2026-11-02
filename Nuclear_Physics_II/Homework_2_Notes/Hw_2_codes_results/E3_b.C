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


void E3b() 


{

TCanvas *c1 = new TCanvas("c1", "Plot", 800, 600);

// define constants 
double lambda_1 = (1.0/.135);
double lambda_2 = (3.0/.135);
double beta_1 = sqrt(1.0 - (1.0/(lambda_1 * lambda_1)));
double beta_2 = sqrt(1.0 - (1.0/(lambda_2 * lambda_2)));


//define lamda-function TF1 for E_pi = 1 GeV; cos -> x[0] in this case
TF1 *f1 = new TF1("f1", [](double *x, double *p) {

double p_1 = ((p[0]*p[0])/2.0); 
double p_2 = p[1]*p[0]*p[0]*((x[0] - p[1])/(1.0 - p[1]*x[0])); 
double p_3 = (pow(p[0],2)*pow(p[1],2)/2.0)*pow(((x[0] - p[1])/(1.0 - p[1]*x[0])), 2); 
return (p_1 + p_2 + p_3); 
}, -1, 1, 2); 

f1->SetParameters(lambda_1, beta_1);
f1->SetLineColor(kBlue);
f1->SetLineWidth(2);
f1->SetMinimum(0.0);
f1->SetMaximum(1.1);
f1->SetTitle("Pion per Cosine Plot;  cos#theta; #frac{dn}{d(cos#theta)}");
f1->Draw();




//define lamda-function TF1 for E_pi = 3 GeV; cos -> x[0] in this case
TF1 *f3 = new TF1("f3", [](double *x, double *p) {

double p_1 = ((p[0]*p[0])/2.0); 
double p_2 = p[1]*p[0]*p[0]*((x[0] - p[1])/(1.0 - p[1]*x[0])); 
double p_3 = (pow(p[0],2)*pow(p[1],2)/2.0)*pow(((x[0] - p[1])/(1.0 - p[1]*x[0])), 2); 
return (p_1 + p_2 + p_3); 
}, -1, 1, 2); 

f3->SetParameters(lambda_2, beta_2);
f3->SetLineColor(kRed);
f3->SetLineWidth(2);
f3->SetMinimum(0.0);
f3->SetMaximum(1.1);
f3->Draw("SAME");



  // Legend
    TLegend *leg = new TLegend(0.6, 0.15, 0.85, 0.35);
    leg->AddEntry(f1,   "E_{#pi^{0}} = 1 GeV", "l");
    leg->AddEntry(f3,   "E_{#pi^{0}} = 3 GeV", "l");
    leg->Draw();




 c1->SaveAs("E3b.root");
}



































































