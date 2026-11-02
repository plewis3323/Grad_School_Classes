#include <iostream> 
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
#include <vector>
#include <string>
#include <TLegend.h>
#include <cmath>
#include <TLatex.h>
using namespace std; 

// Energy calibration function
double Energy_Calibration(double Channel_N) {
    double a = 0.193356;
    double b = -26.0566;
    return (a * Channel_N + b); 
}

// Efficiency for high energy (750-1500 keV)
double Eff_H(double E) {
    double a2 = -3.66794e-05;
    double b2 = 0.120163;
    return (a2 * E  + b2); 
} 

// Efficiency for low energy (0-500 keV)
double Eff_L(double x) {
    double a1 = 1.38049e-06; 
    double b1 =  -0.00265069;
    double c1 = 1.12314; 
    return (a1 * x * x + b1 * x + c1); 
}

void UR(TString filename, TString histname, TCanvas* Sources1) {
    Int_t bin, data;
    ifstream infile;
    Int_t minimum = kMaxInt;
    Int_t maximum = kMinInt;

    infile.open(filename);
    if (!infile.good()) {
        cerr << "Couldn't open file " << filename << "." << endl;
        infile.close();
        return;
    }

    while (!infile.eof()) {
        infile >> bin >> data;
        if (bin > maximum) maximum = bin;
        if (bin < minimum) minimum = bin;
    }

    infile.clear();
    infile.seekg(0, ios::beg);

    TH1F *myhist = new TH1F(histname, histname, maximum - minimum + 1, minimum - 0.5, maximum + 0.5);

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

    // ****************** U-235 Peaks ******************
    // Peak 2: 850-890 (U-235)
    TF1 *gausFit2 = new TF1("gausFit2", "gaus", 865, 880); 
    myhist->Fit(gausFit2, "R"); 
    gausFit2->SetLineColor(16);
    gausFit2->Draw("SAME");
    double N_t2 = gausFit2->Integral(865, 880);
    double N_b2 = (myhist->Integral(850, 865) + myhist->Integral(880, 895)) / 2;
    double N_s2 = N_t2 - N_b2;
    double energy2 = 143.76;   // Energy from INL gamma-ray source
    cout << "U-235 Peak 2 total counts (N_t2): " << N_t2 << endl;
    cout << "U-235 Peak 2 background counts (N_b2): " << N_b2 << endl;
    cout << "U-235 Peak 2 signal counts (N_s2): " << N_s2 << endl;

    // Peak 3: 950-990 (U-235)
    TF1 *gausFit3 = new TF1("gausFit3", "gaus", 969, 980); 
    myhist->Fit(gausFit3, "R"); 
    gausFit3->SetLineColor(17);
    gausFit3->Draw("SAME");
    double N_t3 = gausFit3->Integral(969, 980);
    double N_b3 = (myhist->Integral(958, 969) + myhist->Integral(980, 991)) / 2;
    double N_s3 = N_t3 - N_b3;
    double energy3 = 163.33;   // Energy from INL gamma-ray source
    cout << "U-235 Peak 3 total counts (N_t3): " << N_t3 << endl;
    cout << "U-235 Peak 3 background counts (N_b3): " << N_b3 << endl;
    cout << "U-235 Peak 3 signal counts (N_s3): " << N_s3 << endl;

    // Peak a: 1080-1105 (U-235)
    TF1 *gausFitb = new TF1("gausFitb", "gaus", 1080, 1105); 
    myhist->Fit(gausFitb, "R"); 
    gausFitb->SetLineColor(15);
    gausFitb->Draw("SAME");
    cout << "Mean = " << gausFitb->GetParameter(1) << endl; 
    double N_ta = gausFitb->Integral(1080, 1105);
    cout << "N_t = " << N_ta << endl; 
    double N_ba = (myhist->Integral(1055, 1080) + myhist->Integral(1105, 1130)) / 2; 
    cout << "N_b = " << N_ba << endl; 
    double N_sa = N_ta - N_ba; 
    cout << "N_s = " << N_sa << endl; 
    double std_deva = gausFitb->GetParameter(2); 
    cout << "std error = " << std_deva / sqrt(N_sa) << endl;
    double energya = 185.715; 



    // ****************** U-238 Peaks ******************
    
    
    // Peak 5: 1460-1475 (U-235)
    TF1 *gausFit5 = new TF1("gausFit5", "gaus", 1460, 1475); 
    myhist->Fit(gausFit5, "R"); 
    gausFit5->SetLineColor(9);
    gausFit5->Draw("SAME");
    double N_t5 = gausFit5->Integral(1460, 1475);
    double N_b5 = (myhist->Integral(1440, 1460) + myhist->Integral(1475, 1490)) / 2;
    double N_s5 = N_t5 - N_b5;
    double energy5 = 258.26;  // Energy from INL gamma-ray source
    cout << "U-235 Peak 5 total counts (N_t5): " << N_t5 << endl;
    cout << "U-235 Peak 5 background counts (N_b5): " << N_b5 << endl;
    cout << "U-235 Peak 5 signal counts (N_s5): " << N_s5 << endl;

    // Peak 6: 3965-3987 (U-238)
    TF1 *gausFit6 = new TF1("gausFit6", "gaus", 3965, 3987); 
    myhist->Fit(gausFit6, "R"); 
    gausFit6->SetLineColor(20);
    gausFit6->Draw("SAME");
    double N_t6 = gausFit6->Integral(3965, 3987);
    double N_b6 = (myhist->Integral(3945, 3965) + myhist->Integral(3987, 4005)) / 2;
    double N_s6 = N_t6 - N_b6;
    double energy6 = 742.81;   // Energy from INL gamma-ray source for U-238
    cout << "U-238 Peak 6 total counts (N_t6): " << N_t6 << endl;
    cout << "U-238 Peak 6 background counts (N_b6): " << N_b6 << endl;
    cout << "U-238 Peak 6 signal counts (N_s6): " << N_s6 << endl;

    // Peak 7: 4194-4210 (U-238)
    TF1 *gausFit7 = new TF1("gausFit7", "gaus", 4194, 4210); 
    myhist->Fit(gausFit7, "R"); 
    gausFit7->SetLineColor(21);
    gausFit7->Draw("SAME");
    double N_t7 = gausFit7->Integral(4194, 4210);
    double N_b7 = (myhist->Integral(4174, 4194) + myhist->Integral(4210, 4230)) / 2;
    double N_s7 = N_t7 - N_b7;
    double energy7 = 786.27;  // Energy from INL gamma-ray source for U-238
    cout << "U-238 Peak 7 total counts (N_t7): " << N_t7 << endl;
    cout << "U-238 Peak 7 background counts (N_b7): " << N_b7 << endl;
    cout << "U-238 Peak 7 signal counts (N_s7): " << N_s7 << endl;

    // Peak 8: 5280-5340 (U-238 additional peak)
    TF1 *gausFit8 = new TF1("gausFit8", "gaus", 5280, 5340); 
    myhist->Fit(gausFit8, "R"); 
    gausFit8->SetLineColor(2);
    gausFit8->Draw("SAME");
    double N_t8 = gausFit8->Integral(5280, 5340);
    double N_b8 = (myhist->Integral(5220, 5280) + myhist->Integral(5340, 5400)) / 2;
    double N_s8 = N_t8 - N_b8;
    double energy8 = 1001.7;   // Energy from INL gamma-ray source for U-238
    cout << "U-238 Peak 8 total counts (N_t8): " << N_t8 << endl;
    cout << "U-238 Peak 8 background counts (N_b8): " << N_b8 << endl;
    cout << "U-238 Peak 8 signal counts (N_s8): " << N_s8 << endl;

    // Peak 9: 4060-4140 (U-238)
    TF1 *gausFit9 = new TF1("gausFit9", "gaus", 4060, 4140); 
    myhist->Fit(gausFit9, "R"); 
    gausFit9->SetLineColor(4);
    gausFit9->Draw("SAME");
    double N_t9 = gausFit9->Integral(4060, 4140);
    double N_b9 = (myhist->Integral(4000, 4060) + myhist->Integral(4140, 4200)) / 2;
    double N_s9 = N_t9 - N_b9;
    double energy9 = 766.36;   // Energy from INL gamma-ray source for U-238
    cout << "U-238 Peak 9 total counts (N_t9): " << N_t9 << endl;
    cout << "U-238 Peak 9 background counts (N_b9): " << N_b9 << endl;
    cout << "U-238 Peak 9 signal counts (N_s9): " << N_s9 << endl;

     // Efficiency correction based on energy values
    double E_ff35 = Eff_L(185.715); // Low-energy peak
    double E_ff38 = Eff_H(1001.7); // High-energy peak
    cout << "U235 eff: "<< E_ff35 << "U238 eff " << E_ff38 << endl;; 
    

    // Recalibrated counts (apply to all peaks)
    double N_cal35 = N_sa/E_ff35;
    double N_cal38 = N_s8/E_ff38;
      cout << "U235 count: "<< N_cal35 << "U238 count: " << N_cal38 << endl;
      
      
     double BR_235 = 57.2;
     double BR_238 = 0.842;
     cout <<"The branching ratios 235 and 238: " << BR_235 << " +/-(9)" << " , " << BR_238 << " +/- (8) " << endl;


    // Decay counts for each peak
    double N_d35 = N_cal35 /BR_235;
    double N_d38 = N_cal38 /BR_238;
  
    // Summing decay counts for U-235 and U-238
    double N_sum235 = N_d35;  
    double N_sum238 = N_d38; 
    cout << "Total U-235 decay counts (N_sum235): " << N_sum235 << endl;
    cout << "Total U-238 decay counts (N_sum238): " << N_sum238 << endl;

    // Decay constants for U-235 and U-238
    double lambda235 = log(2) / (7.0338e8);  // U-235 decay constant
    double lambda238 = log(2) / (4.469e9);   // U-238 decay constant
    cout << "Decay constants: lambda235 = " << lambda235 << ", lambda238 = " << lambda238 << endl;

    // Number of atoms for U-235 and U-238
    double N_atoms_235 = N_sum235 / lambda235;
    double N_atoms_238 = N_sum238 / lambda238;
    cout << "Number of atoms: N_atoms_235 = " << N_atoms_235 << ", N_atoms_238 = " << N_atoms_238 << endl;

    // Isotopic ratio U-235 / U-238
    double isotopic = N_atoms_235 / N_atoms_238;
    cout << "The isotopic ratio (U-235 / U-238) =  " << isotopic << endl; 
}

void Run1() {
    // Canvas for reference histograms 
    TCanvas *c1 = new TCanvas("c1");
    cout << "Canvas c1 created for Uranium" << endl;
    UR("Uranium.xy", "Uranium", c1);

    TFile *Ur1 = new TFile("Ur_Run1.root", "RECREATE"); 
    cout << "File Ur_Run1.root created" << endl;

    c1->Write();
    Ur1->Close();
    cout << "Run 1 completed, results saved to Ur_Run1.root" << endl;
}




















