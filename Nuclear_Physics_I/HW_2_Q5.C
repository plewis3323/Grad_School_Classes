

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TStyle.h"

struct Row { double j, mu; };

static double ParseJ(const std::string& spin) {
  std::string s; for (char c: spin) if (c!='(' && c!=')') s.push_back(c);
  for (size_t i=0;i<s.size();++i) if (isdigit(s[i])) {
    size_t k=i; while (k<s.size() && isdigit(s[k])) ++k;
    if (k<s.size() && s[k]=='/' && k+1<s.size()) {
      size_t m=k+1; while (m<s.size() && isdigit(s[m])) ++m;
      double num = atof(s.substr(i,k-i).c_str());
      double den = atof(s.substr(k+1,m-(k+1)).c_str());
      return den? num/den : NAN;
    }
    return atof(s.substr(i,k-i).c_str());
  }
  return NAN;
}

void SchmidtTiny(const char* csv="data/oddZ_moments_clean.csv") {
  const double gL = 1.0;   // proton orbital
  const double gS = 5.586; // proton spin

  std::ifstream fin(csv);
  if (!fin) { printf("Cannot open %s\n", csv); return; }
  std::string line; std::getline(fin,line); // skip header

  std::vector<Row> data;
  while (std::getline(fin,line)) {
    if (line.empty()) continue;
    std::stringstream ss(line);
    std::string Nuc,A,Z,Spin,Mu;
    if (!std::getline(ss,Nuc,',')) continue;
    if (!std::getline(ss,A,  ',')) continue;
    if (!std::getline(ss,Z,  ',')) continue;
    if (!std::getline(ss,Spin,',')) continue;
    if (!std::getline(ss,Mu, ',')) continue;
    double j  = ParseJ(Spin);
    double mu = Mu.empty() ? NAN : atof(Mu.c_str());
    if (!std::isnan(j) && !std::isnan(mu))
      data.push_back({ std::round(j*2)/2.0, mu });
  }

  // build Schmidt curves
  std::vector<double> xj, muPlus, muMinus;
  for (double j : {0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0}) {
    double mup = gL*(j-0.5) + 0.5*gS;
    double mum = (j/(j+1.0))*((j+1.5)*gL - 0.5*gS);
    xj.push_back(j); muPlus.push_back(mup); muMinus.push_back(mum);
  }

  // experimental
  std::vector<double> xexp, yexp;
  for (auto& r: data) { xexp.push_back(r.j); yexp.push_back(r.mu); }

  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("c","Odd-Z μ vs Schmidt",900,650);
  TGraph* gP = new TGraph((int)xj.size(),  xj.data(),  muPlus.data());
  TGraph* gM = new TGraph((int)xj.size(),  xj.data(),  muMinus.data());
  TGraph* gE = new TGraph((int)xexp.size(),xexp.data(),yexp.data());

  gP->SetLineColor(kBlue+1); gP->SetLineWidth(3);
  gM->SetLineColor(kBlue+1); gM->SetLineStyle(2); gM->SetLineWidth(3);
  gE->SetMarkerStyle(20); gE->SetMarkerColor(kRed+1);

  gP->SetTitle("Odd-Z #mu vs Schmidt limits; j; #mu ( #mu_{N} )");
  gP->Draw("AL"); 
  gP->GetYaxis()->SetRangeUser(-7, 7); 
  gM->Draw("L SAME"); 
  gE->Draw("P SAME");

  auto leg = new TLegend(0.15,0.75,0.6,0.9);
  leg->AddEntry(gE,"Experiment","p");
  leg->AddEntry(gP,"Schmidt j=l+1/2","l");
  leg->AddEntry(gM,"Schmidt j=l-1/2","l");
  leg->Draw();

  c->SaveAs("root_outputs/oddZ_schmidt_plot.root");
  printf("Saved plot: oddZ_schmidt_plot\n");
}





































































































































