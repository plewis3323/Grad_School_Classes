// SchmidtTiny.C — minimal ROOT macro for odd-Z Schmidt comparison
// Assumes CSV header: Nucleus,A,Z,Spin,mu_exp (muN)
// Output: oddZ_schmidt_plot.png
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
  // Extract first j from "3/2+", "7/2-", "(1/2+)", "4 or 5", etc.
  std::string s; s.reserve(spin.size());
  for (char c: spin) if (c!='(' && c!=')') s.push_back(c);
  // try fraction a/b
  for (size_t i=0;i<s.size();++i) if (isdigit(s[i])) {
    // read number
    size_t k=i; while (k<s.size() && isdigit(s[k])) ++k;
    if (k<s.size() && s[k]=='/' && k+1<s.size()) {
      size_t m=k+1; while (m<s.size() && isdigit(s[m])) ++m;
      double num = atof(s.substr(i,k-i).c_str());
      double den = atof(s.substr(k+1,m-(k+1)).c_str());
      return den? num/den : NAN;
    }
    // else integer
    return atof(s.substr(i,k-i).c_str());
  }
  return NAN;
}

void SchmidtTiny(const char* csv="data/oddZ_moments_clean.csv") {
  // ---- choose g-factors (proton Schmidt):
  const double gL = 1.0;       // orbital g for proton
  const double gS = 5.586;     // spin g for proton

  // read CSV
  std::ifstream fin(csv);
  if (!fin) { printf("Cannot open %s\n", csv); return; }
  std::string line; std::getline(fin,line); // header
  std::vector<Row> data;
  while (std::getline(fin,line)) {
    if (line.empty()) continue;
    std::stringstream ss(line);
    std::string Nuc,A,Z,Spin,Mu; // 5 columns expected
    if (!std::getline(ss,Nuc,',')) continue;
    if (!std::getline(ss,A,  ',')) continue;
    if (!std::getline(ss,Z,  ',')) continue;
    if (!std::getline(ss,Spin,',')) continue;
    if (!std::getline(ss,Mu, ',')) continue;
    double j  = ParseJ(Spin);
    double mu = Mu.empty() ? NAN : atof(Mu.c_str());
    if (!std::isnan(j) && !std::isnan(mu)) data.push_back({ std::round(j*2)/2.0, mu });
  }
  if (data.empty()) { printf("No usable rows\n"); return; }

  // unique j’s present
  std::vector<double> jvals;
  for (auto& r: data) {
    if (jvals.empty() || jvals.back()!=r.j) {
      bool seen=false; for (double x: jvals) if (fabs(x-r.j)<1e-9) { seen=true; break; }
      if (!seen) jvals.push_back(r.j);
    }
  }
  std::sort(jvals.begin(), jvals.end());

  // build Schmidt curves (exact formulas you wrote):
  // j = l + 1/2:  mu+ = gL*(j-1/2) + 0.5*gS
  // j = l - 1/2:  mu- = (j/(j+1)) * ( (j+3/2)*gL - 0.5*gS )
  std::vector<double> xj, muPlus, muMinus;
  for (double j: jvals) {
    if (j<=0) continue;
    double mup = gL*(j-0.5) + 0.5*gS;
    double mum = (j/(j+1.0)) * ( (j+1.5)*gL - 0.5*gS );
    xj.push_back(j); muPlus.push_back(mup); muMinus.push_back(mum);
  }

  // experimental scatter
  std::vector<double> xexp, yexp; xexp.reserve(data.size()); yexp.reserve(data.size());
  for (auto& r: data) { xexp.push_back(r.j); yexp.push_back(r.mu); }

  // plot
  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("c","Odd-Z μ vs Schmidt (protons)",900,650);
  TGraph* gP = new TGraph((int)xj.size(),  xj.data(),  muPlus.data());
  TGraph* gM = new TGraph((int)xj.size(),  xj.data(),  muMinus.data());
  TGraph* gE = new TGraph((int)xexp.size(),xexp.data(),yexp.data());

  gP->SetLineColor(kBlue+1); gP->SetLineWidth(3);
  gM->SetLineColor(kBlue+1); gM->SetLineStyle(2); gM->SetLineWidth(3);
  gE->SetMarkerStyle(20); gE->SetMarkerSize(1.1); gE->SetMarkerColor(kRed+1);

  gP->SetTitle("Odd-Z ground-state #mu vs Schmidt limits; j (from Spin); #mu ( #mu_{N} )");
  gP->Draw("AL"); gM->Draw("L SAME"); gE->Draw("P SAME");

  auto leg = new TLegend(0.14,0.74,0.62,0.89);
  leg->AddEntry(gE,"Experimental (sample)","p");
  leg->AddEntry(gP,Form("Schmidt j=ℓ+1/2 (g_l=%.3g, g_s=%.3g)",gL,gS),"l");
  leg->AddEntry(gM,"Schmidt j=ℓ-1/2","l");
  leg->Draw();

  c->SetGrid();
  c->SaveAs("root_outputs/oddZ_schmidt_plot.root");
  printf("Saved plot: oddZ_schmidt_plot.png\n");
}

// autorun
void SchmidtTiny() { SchmidtTiny("data/oddZ_moments_clean.csv"); }













































































































































