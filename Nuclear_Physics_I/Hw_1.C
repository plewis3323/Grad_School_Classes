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
#include <string>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <cmath>
using namespace std; 



// === Globals so TF1 can access A,Z,Y by point index ===
static std::vector<int> gA_i, gZ_i;
static std::vector<double> gY_i, gEY_i;



// ---------- simple vectors + loader (INSERTED HERE) ----------
struct AMEVectors {
  vector<double> A, Z, N, BEA, dBEA;
};

// Reads the formatted table: N Z A EL ME dME BEA dBEA AMU (one header line)
inline bool LoadAMEtxt(const char* path, AMEVectors& v, bool dropAeq1=true) {
  ifstream fin(path);
  if (!fin) { cerr << "Cannot open " << path << "\n"; return false; }

  string header; getline(fin, header);  // skip header

  int Ni, Zi, Ai; string EL;
  double ME, dME, BEAi, dBEAi, AMU;

  v = AMEVectors{}; // clear all vectors
  while (fin >> Ni >> Zi >> Ai >> EL >> ME >> dME >> BEAi >> dBEAi >> AMU) {
    if (dropAeq1 && Ai <= 1) continue;
    v.N.push_back((double)Ni);
    v.Z.push_back((double)Zi);
    v.A.push_back((double)Ai);
    v.BEA.push_back(BEAi/1000.0);
    v.dBEA.push_back((dBEAi > 0 ? dBEAi : 100.0)/1000.0); // tiny floor if 0
  }
  return !v.A.empty();
}
// --------------------------------------------------------------

// Pairing function here 
double PairingTermOverA(int A, int Z, double ap) {
  int N = A - Z;

  // figure out the sign: +1 (even-even), 0 (odd A), -1 (odd-odd)
  double sign;
  if (A % 2 != 0) {
    sign = 0.0;                 // odd A -> no pairing
  } else if ((Z % 2 == 0) && (N % 2 == 0)) {
    sign = +1.0;                // even-even
  } else {
    sign = -1.0;                // odd-odd
  }

  // δ(A,Z) = sign * ap / sqrt(A);   contribute δ/A to BE/A
  double delta = sign * ap / sqrt((double)A);
  return delta / (double)A;     // this is δ/A
}

// Put SEM Function here: 
// Beginner-friendly SEMF that returns BE/A (same units as inputs).
// Parameters (typical in MeV): av~15.8, as~17.0, ac~0.70, aa~23.0, ap~34.0
// If your data are in keV, pass these *in keV* instead (×1000).
double SEMF_BEA(int A, int Z,
                double av, double as, double ac, double aa, double ap)
{
  if (A <= 0) return 0.0;

  // Cast to double once to avoid integer math
  double Ad = (double)A;
  double Zd = (double)Z;

  // Common powers of A
  double A13  = pow(Ad, 1.0/3.0);   // A^(1/3)
  double A_13 = 1.0 / A13;          // A^(-1/3)
  double A_1  = 1.0 / Ad;           // 1/A
  double A_43 = A_1 * A_13;          // A^(-4/3) = (1/A)*A^(1/3)

  // Asymmetry I = A - 2Z
  double I = Ad - 2.0*Zd;

  // Terms: volume, surface, Coulomb, asymmetry, pairing
  double termV = av;
  double termS = -as * A_13;
  double termC = -ac * Zd * Zd * A_43;
  double termA = -aa * (I*I) * (A_1*A_1);
  double termP =  PairingTermOverA(A, Z, ap);  // this is δ/A

  return termV + termS + termC + termA + termP;
}



// TF1 wrapper: x = point index i; params p[0..4] = (av,as,ac,aa,ap)
double SEMF_idx(double* x, double* p) {
  int i = (int)std::lround(x[0]);
  if (i < 0 || i >= (int)gA_i.size()) return 0.0;
  return SEMF_BEA(gA_i[i], gZ_i[i], p[0], p[1], p[2], p[3], p[4]);
}


void Energy_Calibration (TCanvas* ECalibration) {
  ECalibration->cd(); 

  // ---- Load data into vectors (A, Z, N, BE/A, d(BE/A)) ----
  AMEVectors dat;
  if (!LoadAMEtxt("data/AME2012massesExptOnly_formatted.txt", dat)) return;
  const int n = (int)dat.A.size();

 // ==== Prepare data for TF1 (index trick) ====
  gA_i.resize(n); gZ_i.resize(n); gY_i.resize(n); gEY_i.resize(n);
  for (int i=0;i<n;++i) {
    //if ((int)dat.A[i] < 20) continue;   // before filling gA_i, etc.
    gA_i[i]  = (int)std::lround(dat.A[i]);
    gZ_i[i]  = (int)std::lround(dat.Z[i]);
    gY_i[i]  = dat.BEA[i];     // keV
    gEY_i[i] = dat.dBEA[i];    // keV
  }

  // Graph vs index: x=i, y=BE/A
  TGraphErrors* gr = new TGraphErrors(n);
  for (int i=0;i<n;++i) {
    gr->SetPoint(i, i, gY_i[i]);
    gr->SetPointError(i, 0.0, gEY_i[i]);
  }
  gr->SetTitle("SEMF fit to BE/A;point index;BE/A (MeV/A)");
  gr->SetMarkerStyle(21);
  gr->SetMarkerSize(0.8);
  gr->Draw("AP");

  // TF1: 5-parameter SEMF (start values in keV to match file)
  TF1 f("SEMF", SEMF_idx, 0, n-1, 5);
  f.SetParNames("a_v","a_s","a_c","a_a","a_p");
  f.SetParameters(15.56, 17.23, 0.70, 23.285, 11.0);
  gr->Fit(&f, "QW");  // quiet + use y-errors as weights

  // Print parameters and chi2/ndf
  std::cout << "\n=== 5-parameter SEMF (BE/A, MeV) ===\n";
  for (int i=0;i<5;++i)
    std::cout << f.GetParName(i) << " = " << f.GetParameter(i)
              << " +- " << f.GetParError(i) << "\n";
  std::cout << "chi2/ndf = " << f.GetChisquare() << " / " << f.GetNDF()
            << " = " << (f.GetNDF()? f.GetChisquare()/f.GetNDF():0) << "\n";

  // (nice) print the function with numbers
  std::cout << std::fixed;
  std::cout << "\nFit function (BE/A in MeV):\n"
            << "BE/A = " << f.GetParameter(0)
            << " - "   << f.GetParameter(1) << "*A^(-1/3)"
            << " - "   << f.GetParameter(2) << "*Z(Z-1)*A^(-4/3)"
            << " - "   << f.GetParameter(3) << "*((A-2Z)^2/A^2)"
            << " + ("  << f.GetParameter(4) << "/sqrt(A))/A * sign\n"
            << "  (sign: +1 even-even, 0 odd-A, -1 odd-odd)\n";

  // Residuals r_i = data - fit
  std::vector<double> res(n);
  for (int i=0;i<n;++i) res[i] = gY_i[i] - f.Eval(i);

  // Residuals vs N
  TCanvas* cN = new TCanvas("cN","Residuals vs N",900,600);
  TGraphErrors* gNres = new TGraphErrors(n);
  for (int i=0;i<n;++i) { gNres->SetPoint(i, dat.N[i], res[i]); gNres->SetPointError(i, 0.0, gEY_i[i]); }
  gNres->SetTitle("Fit residuals;Neutron number N;BE/A_{exp} - BE/A_{fit} (MeV/A)");
  gNres->SetMarkerStyle(20); gNres->SetMarkerSize(0.6);
  gNres->Draw("AP");

  // Residuals vs Z
  TCanvas* cZ = new TCanvas("cZ","Residuals vs Z",900,600);
  TGraphErrors* gZres = new TGraphErrors(n);
  for (int i=0;i<n;++i) { gZres->SetPoint(i, dat.Z[i], res[i]); gZres->SetPointError(i, 0.0, gEY_i[i]); }
  gZres->SetTitle("Fit residuals;Proton number Z;BE/A_{exp} - BE/A_{fit} (MeV/A)");
  gZres->SetMarkerStyle(20); gZres->SetMarkerSize(0.6);
  gZres->Draw("AP");

  // Save images (optional)
  //ECalibration->SaveAs("semf_fit.");
  cN->SaveAs("root_outputs/residuals_vs_N.root");
  cZ->SaveAs("root_outputs/residuals_vs_Z.root");
}

void Run1() {
  TCanvas *c1 = new TCanvas("c1"); 
  Energy_Calibration(c1);
  TFile *Histo_data = new TFile("root_outputs/Hw_7.root", "RECREATE"); 
  c1->Write();
  Histo_data->Close(); 
}





























































































































