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







void Delay_S(TString filename, TString histname, TCanvas* Sources1){

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
  
  
  

// Center the X-axis label by adjusting the offset and title size
myhist->GetXaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
myhist->GetXaxis()->SetTitleSize(0.05);  // Adjust the size if necessary
// Center the Y-axis label by adjusting the offset and title size
myhist->GetYaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
myhist->GetYaxis()->SetTitleSize(0.05);  // Adjust the size if necessary
// Set the X-axis and Y-axis labels
myhist->GetXaxis()->SetTitle("Channel Number");
myhist->GetYaxis()->SetTitle("Counts");

  
  
  
  


infile.close();
  
  
  

Sources1->cd();
// Drawing the histogram on the canvas
myhist->Draw();
// Zoom in on the x-axis
myhist->GetXaxis()->SetRangeUser(0, 10000);  




//Perform a Gaussian Fit #1
TF1 *gausFit1 = new TF1("gausFit1", "gaus", 1477, 1484);
myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit1->SetLineColor(1);
gausFit1->Draw("SAME");
cout << "The Total Counts: N_1 = " << gausFit1->Integral(1477,1484) << endl; 
cout << "Count error 1 = " << sqrt(gausFit1->Integral(1477,1484)) << endl;


//Perform a Gaussian Fit #2
TF1 *gausFit2 = new TF1("gausFit2", "gaus", 2632, 2670);
myhist->Fit(gausFit2, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit2->SetLineColor(2);
gausFit2->Draw("SAME");
cout << "The Total Counts: N_2 = " << gausFit2->Integral(2632, 2670) << endl;
cout << "Count error 2 = " << sqrt(gausFit2->Integral(2632, 2670)) << endl; 



//Perform a Gaussian Fit #3
TF1 *gausFit3 = new TF1("gausFit3", "gaus", 3790, 3838);
myhist->Fit(gausFit3, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit3->SetLineColor(3);
gausFit3->Draw("SAME");
cout << "The Total Counts: N_3 = " << gausFit3->Integral(3790, 3838) << endl; 
cout << "Count error 3 = " << sqrt(gausFit3->Integral(3790, 3838)) << endl; 




//Perform a Gaussian Fit #4
TF1 *gausFit4 = new TF1("gausFit4", "gaus", 4943, 5005);
myhist->Fit(gausFit4, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit4->SetLineColor(4);
gausFit4->Draw("SAME");
cout << "The Total Counts: N_4 = " << gausFit4->Integral(4943, 5005) << endl; 
cout << "Count error 4 = " << sqrt(gausFit4->Integral(4943, 5005)) << endl; 



//Perform a Gaussian Fit #5
TF1 *gausFit5 = new TF1("gausFit5", "gaus", 6030, 6100);
myhist->Fit(gausFit5, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit5->SetLineColor(5);
gausFit5->Draw("SAME");
cout << "The Total Counts: N_5 = " << gausFit5->Integral(6030, 6100) << endl; 
cout << "Count error 5 = " << sqrt(gausFit5->Integral(6030, 6100)) << endl; 




//Perform a Gaussian Fit #6
TF1 *gausFit6 = new TF1("gausFit6", "gaus", 6210, 6275);
myhist->Fit(gausFit6, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit6->SetLineColor(6);
gausFit6->Draw("SAME");
cout << "The Total Counts: N_6 = " << gausFit6->Integral(6210, 6275) << endl; 
cout << "Count error 6 = " << sqrt(gausFit6->Integral(6210, 6275)) << endl; 




//Perform a Gaussian Fit #7
TF1 *gausFit7 = new TF1("gausFit7", "gaus", 6745, 6805);
myhist->Fit(gausFit7, "R");  // "R" means fit in the specified range
// Draw the fit
gausFit7->SetLineColor(7);
gausFit7->Draw("SAME");
cout << "The Total Counts: N_7 = " << gausFit7->Integral(6745, 6805) << endl; 
cout << "Count error 7 = " << sqrt(gausFit7->Integral(6745, 6805)) << endl; 




// Add a legend to the canvas
TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
legend->AddEntry(gausFit1, "t = 0.44 #mus", "l");
legend->AddEntry(gausFit2, "t = 1.50 #mus", "l");
legend->AddEntry(gausFit3, "t = 2.80 #mus", "l");
legend->AddEntry(gausFit4, "t = 4.0 #mus", "l");
legend->AddEntry(gausFit5, "t = 5.14 #mus", "l");
legend->AddEntry(gausFit6, "t = 6.30 #mus", "l");
legend->AddEntry(gausFit7, "t = 6.98 #mus", "l");
legend->Draw();


myhist->SetStats(0);








}; 
















void Run1() {


//Canvas for reference histograms 
TCanvas *c1 = new TCanvas("c1");
Delay_S("Delay_1.xy", "Counts via Time Delay", c1); 
TFile *Histo_data = new TFile("Delay_Histograms.root", "RECREATE"); 
c1->Write(); 
Histo_data->Close(); 



} 









































