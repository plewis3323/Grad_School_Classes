

/* 


Q_1.   Determine the mean, RMS, number of counts, and error on the mean for the five prominent peaks in the Na-22, Co-60, and Cs-137 spectra. Quote the gamma-ray energy and uncertainty for each of the peaks; include a citation of your reference source.



*/ 




/* -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



                            Part C.   Isotope Cs-137



---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

*/

//Information of Isotope Cs_137
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
using namespace std; 




void Read_xy_file(TString filename, TString histname){

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
TCanvas *c1 = new TCanvas("c1", "Co-60 spectrum", 800, 600);

// Drawing the histogram on the canvas
myhist->Draw();
    
// Zoom in on the x-axis
myhist->GetXaxis()->SetRangeUser(0, 6000);  





// Perform a Gaussian Fit #1
    TF1 *gausFit1 = new TF1("gausFit1", "gaus", 1950,1990);
    myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
    // Draw the fit
    gausFit1->SetLineColor(kRed);
    gausFit1->Draw("SAME");



//Finding the Number of Counnts from Bins 1950-1990: Peak #1
double Counts = myhist->Integral(1950,1990);
cout << "The number of Counts:  N = " << Counts << endl; 



//Retrieve the parameters for peak #1 with bins 1950-1990
    Double_t mean = gausFit1->GetParameter(1);      // Mean (μ) of the Gaussian Peak#1
    Double_t rms = gausFit1->GetParameter(2);     // Sigma (σ), the standard deviation

    // Retrieve the errors associated with each parameter
    Double_t meanError = gausFit1->GetParError(1);      // Error on the mean
    Double_t sigmaError = gausFit1->GetParError(2);     // Error on the sigma

    // Output the parameters with their errors
    cout << "Mean: " << mean << " ± " << meanError << endl;
    cout << "Sigma (RMS): " << rms << " ± " << sigmaError<< endl;



//Standard error of the mean for Peak #1
    double std_error =  rms/(sqrt(Counts));
    cout << "the standard error of the mean of the Peak : " << std_error << endl; 
    



// Create a legend and add entries
    TLegend *legend = new TLegend(0.5, 0.6, 0.7, 0.8); // Position: (x1, y1, x2, y2)
    legend->SetHeader("Gaussian Fit Parameters","C"); // Optionally add a header
    legend->AddEntry((TObject*)0, Form("N = %.2f ", Counts), "");
    legend->AddEntry((TObject*)0, Form("Mean = %.2f #pm %.2f", mean, meanError), "");
    legend->AddEntry((TObject*)0, Form("RMS = %.2f #pm %.2f", rms, sigmaError), "");
    legend->AddEntry((TObject*)0, Form("Std error  = %.2f ", std_error), "");
    legend->AddEntry((TObject*)0, "E_{#gamma} = 661.657 #pm 0.003  keV", "");

    
// Add the legend to the canvas
  legend->Draw();
//Save the canvas to a file
  c1->SaveAs(histname + ".pdf");

};




void Run1 () {


Read_xy_file("germaniumdet_cs137.xy", "Cs-137");

}

























