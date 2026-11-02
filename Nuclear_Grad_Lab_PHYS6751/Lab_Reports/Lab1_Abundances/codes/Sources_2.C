                                                                                                                                    
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
#include <cmath>
using namespace std; 


//Predefined function that does Exponential decay on right/Gaussian on the right
Double_t expGaus(Double_t *x, Double_t *par) {
    Double_t mean = par[1];
    if (x[0] < mean) {
        return par[0] * TMath::Exp(-(mean - x[0]) / par[2]);
    } else {
        return par[0] * TMath::Exp(-0.5 * TMath::Power((x[0] - mean) / par[3], 2));
    }
}





void Barium(TString filename, TString histname, TCanvas* Sources1){

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
  
  
  

Sources1->cd();
// Drawing the histogram on the canvas
myhist->Draw();
    
// Zoom in on the x-axis
myhist->GetXaxis()->SetRangeUser(0, 6000);  



//Fit #1
TF1 *gausFit1 = new TF1("gausFit1",  expGaus, 500, 600, 4);
gausFit1->SetParameters(681.841, 538.452, 3, 2.81855);  // Adjust initial guesses based on first fit. 
myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit1->SetLineColor(kRed);
gausFit1->Draw("SAME");
//background on Fit #1
TF1 *Bgrd_1 = new TF1("Bgrd_1", "30.0", 525,548);
myhist->Fit(Bgrd_1, "R"); 
Bgrd_1->SetLineColor(1); 
Bgrd_1->Draw("SAME"); 
cout << "Peak 1 Mean = " << gausFit1->GetParameter(1) << endl; 
cout << "Peak 1 total: N_t = " << gausFit1->Integral(500,600) << endl; 
cout << "Peak 1 Background: N_b = " << Bgrd_1->Integral(525,548) << endl; 
cout << "Peak 1 Signal: N_s = " << (gausFit1->Integral(500,600) - Bgrd_1->Integral(525,548)) << endl; 
cout << "Peak 1 std error = " << (gausFit1->GetParameter(2))/sqrt(gausFit1->Integral(500,600) - Bgrd_1->Integral(525,548)) << endl; 



//Fit #2 
TF1 *gausFit2 = new TF1("gausFit2", "gaus", 1550, 1580); 
myhist->Fit(gausFit2, "R"); 
//Draw the fit 
gausFit2->SetLineColor(5);
gausFit2->Draw("SAME");
//background on Fit #2
TF1 *Bgrd_2 = new TF1("Bgrd_2", "10.0", 1555, 1573);
myhist->Fit(Bgrd_2, "R"); 
Bgrd_2->SetLineColor(1); 
Bgrd_2->Draw("SAME"); 
cout << "Peak 2 Mean = " << gausFit2->GetParameter(1) << endl; 
cout << "Peak 2 total: N_t = " << gausFit2->Integral(1550, 1580) << endl; 
cout << "Peak 2 Background: N_b = " << Bgrd_2->Integral(1555, 1573) << endl; 
cout << "Peak 2 Signal: N_s = " << (gausFit2->Integral(1550, 1580) - Bgrd_2->Integral(1555, 1573)) << endl; 
cout << "Peak 2 std error = " << (gausFit2->GetParameter(2))/sqrt(gausFit2->Integral(1550, 1580) - Bgrd_2->Integral(1555, 1573)) << endl; 




//Fit #3 
TF1 *gausFit3 = new TF1("gausFit3", "gaus", 1680, 1715); 
myhist->Fit(gausFit3, "R"); 
//Draw the fit 
gausFit3->SetLineColor(7);
gausFit3->Draw("SAME");
cout << "Peak 3 Mean = " << gausFit3->GetParameter(1) << endl; 
cout << "Peak 3 total: N = " << gausFit3->Integral(1680, 1715) << endl;
cout << "Peak 3 std error = " << (gausFit3->GetParameter(2))/sqrt(gausFit3->Integral(1680, 1715)) << endl; 


//Fit #4
TF1 *gausFit4 = new TF1("gausFit4", "gaus", 1965, 2000); 
myhist->Fit(gausFit4, "R"); 
//Draw the fit 
gausFit4->SetLineColor(9);
gausFit4->Draw("SAME");
cout << "Peak 4 Mean = " << gausFit4->GetParameter(1) << endl; 
cout << "Peak 4 total: N = " << gausFit4->Integral(1965, 2000) << endl;
cout << "Peak 4 std error = " << (gausFit4->GetParameter(2))/sqrt(gausFit4->Integral(1965, 2000)) << endl; 




//Fit #5
TF1 *gausFit5 = new TF1("gausFit5", "gaus", 2110, 2140); 
myhist->Fit(gausFit5, "R"); 
//Draw the fit 
gausFit5->SetLineColor(15);
gausFit5->Draw("SAME");
cout << "Peak 5 Mean = " << gausFit5->GetParameter(1) << endl; 
cout << "Peak 5 total: N = " << gausFit5->Integral(2110, 2140) << endl;
cout << "Peak 5 std error = " << (gausFit5->GetParameter(2))/sqrt(gausFit5->Integral(2110, 2140)) << endl; 


};





void Cobalt(TString filename, TString histname,  TCanvas* Sources2){

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
  
  


Sources2->cd();
// Drawing the histogram on the canvas
myhist->Draw();
    




//Fit #1
TF1 *gausFit1 = new TF1("gausFit1", "gaus", 6160, 6240);
myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit1->SetLineColor(15);
gausFit1->Draw("SAME");
cout << "The Total Counts: N = " << gausFit1->Integral(6160, 6240) << endl; 
    
    

//Fit #2 
TF1 *gausFit2 = new TF1("gausFit2", "gaus", 7000, 7050);
myhist->Fit(gausFit2, "R"); 
//Draw the fit 
gausFit2->SetLineColor(7);
gausFit2->Draw("SAME");
cout << "The Total Counts: N = " << gausFit2->Integral(7000, 7050) << endl; 



}; 







void Cesium(TString filename, TString histname,  TCanvas* Sources3){

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
  
Sources3->cd();
// Drawing the histogram on the canvas
myhist->Draw();
    




// Perform a Gaussian Fit #1
    TF1 *gausFit1 = new TF1("gausFit1", "gaus", 3520,3570);
    myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
    // Draw the fit
    gausFit1->SetLineColor(16);
    gausFit1->Draw("SAME");
    cout << "The Total Counts: N = " << gausFit1->Integral(3250, 3570) << endl; 

  


};









void Run1() {


//Canvas for reference histograms 
TCanvas *c1 = new TCanvas("c1");
TCanvas *c2 = new TCanvas("c2");
TCanvas *c3 = new TCanvas("c3");



Barium("Barium.xy", "Barium", c1); 
Cobalt("Cobalt.xy", "Cobalt", c2); 
Cesium("Cesium.xy", "Cesium", c3); 


TFile *Histo_data = new TFile("Histograms.root", "RECREATE"); 



c1->Write(); 
c2->Write(); 
c3->Write(); 


Histo_data->Close(); 



} 






































































































































































































