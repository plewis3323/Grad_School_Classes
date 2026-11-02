



#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
using namespace std;




struct AMEVectors {
  vector<double> A, Z, N, BEA, dBEA;
};

// AME2012 formatted: N Z A EL ME dME BEA dBEA AMU
inline bool LoadAMEtxt(const char* path, AMEVectors& v, bool dropAeq1=true) {
  ifstream fin(path);
  if (!fin) { cerr << "Cannot open " << path << "\n"; return false; }
  string header; getline(fin, header); // skip header line

  int Ni, Zi, Ai; string EL;
  double ME, dME, BEAi, dBEAi, AMU;

  v = AMEVectors{};
  while (fin >> Ni >> Zi >> Ai >> EL >> ME >> dME >> BEAi >> dBEAi >> AMU) {
    if (dropAeq1 && Ai <= 1) continue;

    // Skip points with zero error
    //if (dBEAi == 0.0) continue;

    v.N.push_back((double)Ni);
    v.Z.push_back((double)Zi);
    v.A.push_back((double)Ai);
    v.BEA.push_back(BEAi/1000.0);    // convert to MeV
    v.dBEA.push_back(dBEAi/1000.0);  // convert to MeV
  }
  return !v.A.empty();
  
}

void SEMF_TF2_Fit(TCanvas* Q7, TCanvas* Res_N, TCanvas* Res_Z) {

  Q7->cd(); 
  // 1) Load data
  AMEVectors dat;
  if (!LoadAMEtxt("AME2012massesExptOnly_formatted.txt", dat)) return;
  
  
  
  const int n = (int)dat.A.size();
  // 2D Graph errors here 
    TGraph2DErrors* graph = new TGraph2DErrors(n);
    graph->SetName("B/A_Graph");
    graph->SetTitle("Binding Energy per Nucleon");
    graph->GetXaxis()->SetTitle("N (neutrons)");
    graph->GetYaxis()->SetTitle("Z (protons)");
    graph->GetZaxis()->SetTitle("B/A (MeV/A)");
    for (int i = 0; i < n; ++i) {
        // Use N (x), Z (y), and B/A (z) for each point
        graph->SetPoint(i, dat.N[i], dat.Z[i], dat.BEA[i]);
        graph->SetPointError(i, 0.0, 0.0, dat.dBEA[i]);
    }
    
  
  //TF2 here
  TF2* SME = new TF2("SME", [](double* Ind_Var, double* param) {
  
  double N = Ind_Var[0]; 
  double Z = Ind_Var[1]; 
  double A = N + Z; 
  
  if (A <= 0) return 0.0; 
  
  
  // Fit parameters SEMF
  double aV = param[0]; 
  double aS = param[1]; 
  double aC = param[2]; 
  double aA = param[3]; 
  double aP = param[4]; 
  
  
  // pairing sign details
  int delta = 0;
  int N_int = static_cast<int>(round(N));
  int Z_int = static_cast<int>(round(Z));
  bool N_ev = (N_int % 2 ==0);
  bool Z_ev = (Z_int % 2 ==0);
  if (N_ev && Z_ev) { 
     delta = 1; 
  }else if (!N_ev && !Z_ev) {
     delta = -1; 
  }else { 
     delta = 0; 
   }
   
  // B/A terms are defined here 
  double Volume = aV; 
  double Surface = -aS*pow(A, -1.0/3.0);
  double Coulumb = -aC*pow(Z, 2.0)*pow(A, -4.0/3.0);
  double Assym = -aA*pow((A - 2*Z),2.0)*pow(A, -2.0);
  //Pairing term Final 
  double pairing = 0.0;
  if (delta !=0){
     pairing = delta * aP * pow(A, -3.0/2.0); 
  } 
  
 double Final_terms = (Volume + Surface + Coulumb + Assym + pairing); 
 return Final_terms;
 
 }, 
 0.0, 300.0, 
 0.0, 300.0, 
 5
); 


  
  

 //graph paramaters here 
// Set initial parameter guesses (approximate values in keV)
SME->SetParameter(0, 15.0);  
SME->SetParameter(1, 16.0); 
SME->SetParameter(2, 0.7);   
SME->SetParameter(3, 20.0);  
SME->SetParameter(4, 10.0);  

graph->Draw("P0");
TFitResultPtr fitResult = graph->Fit(SME, "W");  // use "W" for weighting
SME->SetLineColor(kBlue);
SME->Draw("surf1 same");
graph->SetMarkerStyle(21);  // Set a specific marker style
graph->SetMarkerSize(1.0);  // Set marker size
graph->SetLineColor(15);  // Set line color for error bars
graph->SetLineWidth(7);     // Set line width for error bars


// Center the X-axis label by adjusting the offset and title size
graph->GetXaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetXaxis()->SetTitleSize(0.05);  // Adjust the size if necessary

// Center the Y-axis label by adjusting the offset and title size
graph->GetYaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetYaxis()->SetTitleSize(0.05);  // Adjust the size if necessary


 
  // Legend
  auto leg = new TLegend(0.12, 0.80, 0.42, 0.92);
  leg->AddEntry(graph, "Data (B/A)", "p");
  leg->AddEntry(SME,   "SEMF fit (surf)", "l");
  leg->Draw();

 
// Extract best-fit parameters and uncertainties:
    double aV_fit    = SME->GetParameter(0);
    double aV_err    = SME->GetParError(0);
    double aS_fit    = SME->GetParameter(1);
    double aS_err    = SME->GetParError(1);
    double aC_fit    = SME->GetParameter(2);
    double aC_err    = SME->GetParError(2);
    double aA_fit    = SME->GetParameter(3);
    double aA_err    = SME->GetParError(3);
    double aP_fit    = SME->GetParameter(4);
    double aP_err    = SME->GetParError(4);

 cout << "\nBest-fit parameters:" << endl;
 cout << " aV = " << aV_fit << " ± " << aV_err << " MeV" << endl;
 cout << " aS = " << aS_fit << " ± " << aS_err << " MeV" << endl;
 cout << " aC = " << aC_fit << " ± " << aC_err << " MeV" << endl;
 cout << " aA = " << aA_fit << " ± " << aA_err << " MeV" << endl;
 cout << " aP = " << aP_fit << " ± " << aP_err << " MeV (pairing term)" << endl;
 
 
  
  
  
// Residuals calculations
vector<double> Res(n), U_Res(n), New_B_A(n), New_N(n), New_Z(n); 
for(int j = 0; j < n; j++) {

   New_N[j] = dat.N[j]; 
   New_Z[j] = dat.Z[j];
   New_B_A[j] = SME->Eval(dat.N[j], dat.Z[j]); 
   Res[j] = (dat.BEA[j] - New_B_A[j]); 
   U_Res[j] = dat.dBEA[j]; 
   
   
   
} 




 // Residual plot per N 
    Res_N->cd();
    TGraphErrors* residualGraph_N = new TGraphErrors(n, New_N.data(), Res.data(), nullptr, U_Res.data());
    residualGraph_N->SetTitle("Residuals vs N (neutron number)");
    residualGraph_N->SetMarkerStyle(21);
    residualGraph_N->SetMarkerSize(1.0);
    residualGraph_N->SetMarkerColor(kRed);

    residualGraph_N->GetXaxis()->SetTitle("N (neutron number)");
    residualGraph_N->GetYaxis()->SetTitle("Residuals (MeV/A)");
    residualGraph_N->Draw("AP");

    // Draw a horizontal line at y = 0 using TLine
    TLine* zeroLine = new TLine(2, 0, 6.5, 0); // Line spans the range of x-values
    zeroLine->SetLineColor(kBlack);
    zeroLine->SetLineWidth(2);
    zeroLine->Draw("SAME");
    
    
    
    
     // Residual plot per Z
    Res_Z->cd();
    TGraphErrors* residualGraph_Z = new TGraphErrors(n, New_Z.data(), Res.data(), nullptr, U_Res.data());
    residualGraph_Z->SetTitle("Residuals vs Z (proton number)");
    residualGraph_Z->SetMarkerStyle(21);
    residualGraph_Z->SetMarkerSize(1.0);
    residualGraph_Z->SetMarkerColor(kBlack);

    residualGraph_Z->GetXaxis()->SetTitle("Z (proton number)");
    residualGraph_Z->GetYaxis()->SetTitle("Residuals (MeV/A)");
    residualGraph_Z->Draw("AP");

    // Draw a horizontal line at y = 0 using TLine
    TLine* zeroLine_A = new TLine(2, 0, 6.5, 0); // Line spans the range of x-values
    zeroLine_A->SetLineColor(kBlack);
    zeroLine_A->SetLineWidth(2);
    zeroLine_A->Draw("SAME");




  
   // Write to ROOT file
    TFile *Histo_data = new TFile("Final_Q7.root", "RECREATE");
    Q7->Write();
    Res_N->Write();
    Res_Z->Write(); 
    Histo_data->Close();
  
 
  
 
  } 
   
  


void Run1() {


TCanvas *c1 = new TCanvas("c1");
TCanvas *c2 = new TCanvas("c2");
TCanvas *c3 = new TCanvas("c3");
 SEMF_TF2_Fit(c1, c2, c3);

    
}




















































































































