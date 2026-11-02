

/* 


Q_5.   Choose 4-6 prominent peaks in the europium spectrum that span the peak positions from the Na-22, Co-60, and Cs-137 spectra.  Determine the mean, RMS, number of counts, and error on the mean for each peak. Note that many of the peaks in the europium spectra may require background subtraction, so be clear about reporting your total and signal counts and the error on your signal counts.


*/ 




/* -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



                             Isotope  Eu-152



---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

*/

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





void Eu_152(TString filename, TString histname){

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
  
  
// Creating a TCanvas to draw the histogram
TCanvas *c1 = new TCanvas("c1", "Eu-152 spectrum", 800, 600);

// Drawing the histogram on the canvas
myhist->Draw();
    
// Zoom in on the x-axis
myhist->GetXaxis()->SetRangeUser(0, 6000);  




//Peak #1:
TF1 *Fit1 = new TF1("Fit1", "gaus", 310, 340);
myhist->Fit(Fit1, "R");  // "R" means fit in the specified range
Fit1->SetLineColor(kRed);
Fit1->Draw("SAME");

//background Fit
TF1 *Fit2 = new TF1("Fit2", "250.0", 305, 345);
myhist->Fit(Fit2, "R");  // "R" means fit in the specified range
Fit2->SetLineColor(kRed);
Fit2->Draw("SAME");

// Total Count
double N_tCounts1 = Fit1->Integral(310,340);
cout << "The number of Total Counts (Peak #1): N_T = " << N_tCounts1 << endl;

// Background Count from the background fit
double N_bkgd1 = Fit2->Integral(310, 340);
cout << "The number of background Counts (Peak #1): N_b = " << N_bkgd1 << endl;

// Signal Count
double N_sig1 = (N_tCounts1 - N_bkgd1);
cout << "The number of signal Counts (Peak #1): N_s = " << N_sig1 << endl;

// Mean of the signal
double mean1 = Fit1->GetParameter(1);
double mean_err1 = Fit1->GetParError(1);
cout << "The mean (Peak #1): x_bar = " << mean1 << " ± " << mean_err1 << endl;

// RMS
double RMS1 = Fit1->GetParameter(2);
double RMS_err1 = Fit1->GetParError(2);
cout << "The standard deviation (Peak #1): sigma = " << RMS1 << " ± " << RMS_err1 << endl;

// Error of the Mean
double std_error1 = (RMS1 / sqrt(N_sig1));
cout << "The Error of the mean (Peak #1): sigma/(N_sig) = " << std_error1 << endl;

// Error of the Signal Counts
double Counts_err1 = sqrt(N_tCounts1 + N_bkgd1);
cout << "The Count error (Peak #1) = " << Counts_err1 << endl;







//Peak #2:
TF1 *Fit3 = new TF1("Fit3", "gaus", 680, 720);
myhist->Fit(Fit3, "R");
Fit3->SetLineColor(kBlue);
Fit3->Draw("SAME");

//background Fit
TF1 *Fit4 = new TF1("Fit4", "100.0", 660, 740);
myhist->Fit(Fit4, "R");
Fit4->SetLineColor(kBlue);
Fit4->Draw("SAME");

// Total Count
double N_tCounts2 = Fit3->Integral(680,720);
cout << "The number of Total Counts (Peak #2): N_T = " << N_tCounts2 << endl;

// Background Count
double N_bkgd2 = Fit4->Integral(680, 720);
cout << "The number of background Counts (Peak #2): N_b = " << N_bkgd2 << endl;

// Signal Count
double N_sig2 = (N_tCounts2 - N_bkgd2);
cout << "The number of signal Counts (Peak #2): N_s = " << N_sig2 << endl;

// Mean of the signal
double mean2 = Fit3->GetParameter(1);
double mean_err2 = Fit3->GetParError(1);
cout << "The mean (Peak #2): x_bar = " << mean2 << " ± " << mean_err2 << endl;

// RMS
double RMS2 = Fit3->GetParameter(2);
double RMS_err2 = Fit3->GetParError(2);
cout << "The standard deviation (Peak #2): sigma = " << RMS2 << " ± " << RMS_err2 << endl;

// Error of the Mean
double std_error2 = (RMS2 / sqrt(N_sig2));
cout << "The Error of the mean (Peak #2): sigma/(N_sig) = " << std_error2 << endl;

// Error of the Signal Counts
double Counts_err2 = sqrt(N_tCounts2 + N_bkgd2);
cout << "The Count error (Peak #2) = " << Counts_err2 << endl;








//Peak #3:
TF1 *Fit5 = new TF1("Fit5", "gaus", 2320, 2350);
myhist->Fit(Fit5, "R");
Fit5->SetLineColor(kGreen);
Fit5->Draw("SAME");

//background Fit
TF1 *Fit6 = new TF1("Fit6", "25.0", 2310 , 2360);
myhist->Fit(Fit6, "R");
Fit6->SetLineColor(kGreen);
Fit6->Draw("SAME");

// Total Count
double N_tCounts3 = Fit5->Integral(2320, 2350);
cout << "The number of Total Counts (Peak #3): N_T = " << N_tCounts3 << endl;

// Background Count
double N_bkgd3 = Fit6->Integral(2320, 2350);
cout << "The number of background Counts (Peak #3): N_b = " << N_bkgd3 << endl;

// Signal Count
double N_sig3 = (N_tCounts3 - N_bkgd3);
cout << "The number of signal Counts (Peak #3): N_s = " << N_sig3 << endl;

// Mean of the signal
double mean3 = Fit5->GetParameter(1);
double mean_err3 = Fit5->GetParError(1);
cout << "The mean (Peak #3): x_bar = " << mean3 << " ± " << mean_err3 << endl;

// RMS
double RMS3 = Fit5->GetParameter(2);
double RMS_err3 = Fit5->GetParError(2);
cout << "The standard deviation (Peak #3): sigma = " << RMS3 << " ± " << RMS_err3 << endl;

// Error of the Mean
double std_error3 = (RMS3 / sqrt(N_sig3));
cout << "The Error of the mean (Peak #3): sigma/(N_sig) = " << std_error3 << endl;

// Error of the Signal Counts
double Counts_err3 = sqrt(N_tCounts3 + N_bkgd3);
cout << "The Count error (Peak #3) = " << Counts_err3 << endl;







//Peak #4:
TF1 *Fit7 = new TF1("Fit7", "gaus", 4220, 4280);
myhist->Fit(Fit7, "R");
Fit7->SetLineColor(kMagenta);
Fit7->Draw("SAME");

// Total Count
double N_s4 = Fit7->Integral(4220, 4280);
cout << "The number of Total Counts (Peak #4): N_s = " << N_s4 << endl;

// Mean of the signal
double mean4 = Fit7->GetParameter(1);
double mean_err4 = Fit7->GetParError(1);
cout << "The mean (Peak #4): x_bar = " << mean4 << " ± " << mean_err4 << endl;

// RMS
double RMS4 = Fit7->GetParameter(2);
double RMS_err4 = Fit7->GetParError(2);
cout << "The standard deviation (Peak #4): sigma = " << RMS4 << " ± " << RMS_err4 << endl;

// Error of the Mean
double std_error4 = (RMS4 / sqrt(N_s4));
cout << "The Error of the mean (Peak #4): sigma/(N_sig) = " << std_error4 << endl;






// Create a legend
TLegend *legend = new TLegend(0.1, 0.4, 0.5, 0.9);  // Position the legend (adjust coordinates as needed)
legend->SetHeader("Peak Analysis Results", "C");  // Optional: Add a header to the legend

// Peak #1
legend->AddEntry((TObject*)0, Form("Peak #1: Mean = %.2f +/- %.2f", mean1, mean_err1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: RMS = %.2f +/- %.2f", RMS1, RMS_err1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: Total Counts = %.0f", N_tCounts1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: Background Counts = %.0f", N_bkgd1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: Signal Counts = %.0f", N_sig1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: Error (Counts) = %.2f", Counts_err1), "");
legend->AddEntry((TObject*)0, Form("Peak #1: Error of Mean = %.4f", std_error1), "");
legend->AddEntry((TObject*)0, Form("-------------------------------------------"), "");

// Peak #2
legend->AddEntry((TObject*)0, Form("Peak #2: Mean = %.2f +/- %.2f", mean2, mean_err2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: RMS = %.2f +/- %.2f", RMS2, RMS_err2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: Total Counts = %.0f", N_tCounts2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: Background Counts = %.0f", N_bkgd2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: Signal Counts = %.0f", N_sig2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: Error (Counts) = %.2f", Counts_err2), "");
legend->AddEntry((TObject*)0, Form("Peak #2: Error of Mean = %.4f", std_error2), "");
legend->AddEntry((TObject*)0, Form("-------------------------------------------"), "");

// Peak #3
legend->AddEntry((TObject*)0, Form("Peak #3: Mean = %.2f +/- %.2f", mean3, mean_err3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: RMS = %.2f +/- %.2f", RMS3, RMS_err3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: Total Counts = %.0f", N_tCounts3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: Background Counts = %.0f", N_bkgd3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: Signal Counts = %.0f", N_sig3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: Error (Counts) = %.2f", Counts_err3), "");
legend->AddEntry((TObject*)0, Form("Peak #3: Error of Mean = %.4f", std_error3), "");
legend->AddEntry((TObject*)0, Form("-------------------------------------------"), "");

// Peak #4
legend->AddEntry((TObject*)0, Form("Peak #4: Mean = %.2f +/- %.2f", mean4, mean_err4), "");
legend->AddEntry((TObject*)0, Form("Peak #4: RMS = %.2f +/- %.2f", RMS4, RMS_err4), "");
legend->AddEntry((TObject*)0, Form("Peak #4: Signal Counts = %.0f", N_s4), "");
legend->AddEntry((TObject*)0, Form("Peak #4: Error of Mean = %.4f", std_error4), "");
legend->AddEntry((TObject*)0, Form("-------------------------------------------"), "");

// Set text size for better fit in the legend (optional)
legend->SetTextSize(0.01);  // Adjust the text size to fit everything

// Draw the legend on the canvas
legend->Draw();

// Update the canvas to display everything
c1->Update();















}



void Eu() {


Eu_152("germaniumdet_eu152.xy", "Eu-152 Spectrum");


}




























