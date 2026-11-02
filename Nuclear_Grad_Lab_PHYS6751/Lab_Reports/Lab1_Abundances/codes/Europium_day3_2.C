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





void Eu_152(TString filename, TString histname, TCanvas *Eu){

  Int_t bin, data;
  ifstream infile;
  
  Int_t minimum=kMaxInt;
  Int_t maximum=kMinInt;

  infile.open(filename);
  if (! infile.good()){
    cerr << "Couldn't open file " << filename << "." << endl;
    infile.close();
    return;
  }

  while(!infile.eof()){
      infile >> bin >> data;
      if(bin>maximum)
	maximum=bin;
      if(bin<minimum)
	minimum=bin;
    }
    //  infile.close();
    
  cout << "after first loop"<< endl;

  if(minimum>maximum){
    cerr << "Minimum is greater than Maximum" << minimum << maximum
	 << endl;
    return;
  }
  cout << "after test on minimum>maximum"<< endl;


  TH1F *myhist = new TH1F(histname,histname,maximum-minimum+1, minimum-.5, maximum+.5);

  cout << "after making the hist"<< endl;

  infile.clear();
  infile.seekg(0, ios::beg);


  //  infile.open(filename);
  while (!infile.eof()) {
    infile >> bin >> data;
    if (bin%100==0)
      cout << "\t" << bin
	   << "\t" << data << endl;
    for (size_t i=0; i<data; i++){
      myhist->Fill(bin);
    }
  }
  
  
  
  


  infile.close();
  
  
Eu->cd();

// Drawing the histogram on the canvas
myhist->Draw();






//Fit #1
TF1 *gausFit1 = new TF1("gausFit1",  "gaus", 745, 775); 
myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit1->SetLineColor(kRed);
gausFit1->Draw("SAME");
//background on Fit #1
TF1 *Bgrd_1 = new TF1("Bgrd_1", "100.0", 751, 764);
myhist->Fit(Bgrd_1, "R"); 
Bgrd_1->SetLineColor(1); 
Bgrd_1->Draw("SAME"); 
cout << "Peak 1 Mean = " << gausFit1->GetParameter(1) << endl; 
cout << "Peak 1 total: N_t = " << gausFit1->Integral(745,775) << endl; 
cout << "Peak 1 Background: N_b = " << Bgrd_1->Integral(751, 764) << endl; 
cout << "Peak 1 Signal: N_s = " << (gausFit1->Integral(745, 775) - Bgrd_1->Integral(751, 764)) << endl; 
cout << "Peak 1 std error = " << (gausFit1->GetParameter(2))/sqrt(gausFit1->Integral(745, 775) - Bgrd_1->Integral(751, 764)) << endl; 



//Fit #2 
TF1 *gausFit2 = new TF1("gausFit2", "gaus", 1385, 1410); 
myhist->Fit(gausFit2, "R"); 
//Draw the fit 
gausFit2->SetLineColor(5);
gausFit2->Draw("SAME");
//background on Fit #2
TF1 *Bgrd_2 = new TF1("Bgrd_2", "40.0", 1392, 1407);
myhist->Fit(Bgrd_2, "R"); 
Bgrd_2->SetLineColor(1); 
Bgrd_2->Draw("SAME"); 
cout << "Peak 2 Mean = " << gausFit2->GetParameter(1) << endl; 
cout << "Peak 2 total: N_t = " << gausFit2->Integral(1385, 1410) << endl; 
cout << "Peak 2 Background: N_b = " << Bgrd_2->Integral(1392, 1407) << endl; 
cout << "Peak 2 Signal: N_s = " << (gausFit2->Integral(1385, 1410) - Bgrd_2->Integral(1392, 1407)) << endl; 
cout << "Peak 2 std error = " << (gausFit2->GetParameter(2))/sqrt(gausFit2->Integral(1385, 1410) - Bgrd_2->Integral(1392, 1407)) << endl; 



//Fit #3
TF1 *gausFit3 = new TF1("gausFit3", "gaus", 1900, 1940); 
myhist->Fit(gausFit3, "R"); 
//Draw the fit 
gausFit3->SetLineColor(9);
gausFit3->Draw("SAME");
//background on Fit #2
TF1 *Bgrd_3 = new TF1("Bgrd_3", "20.0", 1906, 1924);
myhist->Fit(Bgrd_3, "R"); 
Bgrd_3->SetLineColor(1); 
Bgrd_3->Draw("SAME"); 
cout << "Peak 3 Mean = " << gausFit3->GetParameter(1) << endl; 
cout << "Peak 3 total: N_t = " << gausFit3->Integral(1900, 1940) << endl; 
cout << "Peak 3 Background: N_b = " << Bgrd_3->Integral(1906, 1924) << endl; 
cout << "Peak 3 Signal: N_s = " << (gausFit3->Integral(1900, 1940) - Bgrd_3->Integral(1906, 1924)) << endl; 
cout << "Peak 3 std error = " << (gausFit3->GetParameter(2))/sqrt(gausFit3->Integral(1900, 1940) - Bgrd_3->Integral(1906, 1940)) << endl; 



//Fit #4 
TF1 *gausFit4 = new TF1("gausFit4", "gaus", 2420, 2445); 
myhist->Fit(gausFit4, "R"); 
gausFit4->SetLineColor(13);
gausFit4->Draw("SAME");
cout << "Peak 4 Mean = " << gausFit4->GetParameter(1) << endl; 
double N_t = gausFit4->Integral(2420, 2445);
cout << "Peak 4 total: N_t = " << N_t << endl; 
double N_b = (myhist->Integral(2395, 2420) + myhist->Integral(2445, 2470))/2; 
cout << "Peak 4 Background: N_b = " << N_b << endl; 
double N_s = (N_t - N_b); 
cout << "Peak 4 signal: N_s = " << N_s << endl; 
double std_dev = gausFit4->GetParameter(2); 
cout << "Peak 4 std error = " << std_dev/sqrt(N_s) << endl; 





//Fit #5
TF1 *gausFit5 = new TF1("gausFit5", "gaus", 4140, 4200); 
myhist->Fit(gausFit5, "R"); 
gausFit5->SetLineColor(17);
gausFit5->Draw("SAME");
cout << "Peak 4 Mean = " << gausFit5->GetParameter(1) << endl; 
double N_t1 = gausFit5->Integral(4140, 4200);
cout << "Peak 4 total: N_t = " << N_t1 << endl; 
double N_b1 = (myhist->Integral(4080, 4140) + myhist->Integral(4200, 4260))/2; 
cout << "Peak 4 Background: N_b = " << N_b << endl; 
double N_s1 = (N_t1 - N_b1); 
cout << "Peak 4 signal: N_s = " << N_s1 << endl; 
double std_dev1 = gausFit5->GetParameter(2); 
cout << "Peak 4 std error = " << std_dev1/sqrt(N_s1) << endl; 











}



void Eu() {

TCanvas *c1 = new TCanvas("C1");
Eu_152("Europium.xy", "Europium", c1);
TFile *Eu_day1 = new TFile("Europium_day1.root", "RECREATE"); 
c1->Write();
Eu_day1->Close(); 



}





































