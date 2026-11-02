
/* 


Q_4.  Determine the energy resolution of the detector. Does the energy resolution depend on gamma-ray energy? If so, describe the energy dependence of the energy resolution.


*/ 





/* 


                       Cs-137   Energy Resolutions.
                       

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
using namespace std; 




void Cs_137_Resolution(TString filename, TString histname){

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
    

    

// Known energies in (keV) and corresponding channel numbers
Double_t Cs_Energies[1] = {661.657};
Double_t Cs_Channel_N[1]  = {1972.05};


// Slope and intercept
double a = 0.327956;
    
    
    
// FWHM = 2.355 * RMS    
// FWHM of Peak #1 where  mean channel = 3531.45
double FWHM_1 = 2.355 * rms*a;
cout << "the full width half max of peak #1; FWHM =   " << FWHM_1 << endl; 



// Resolution: R = FWHM/(E_gamma) * 100 
// Resolution of Peak #1 where channel = 3531.45
double R_1 = (FWHM_1/Cs_Energies[0]) * 100; 
cout << " R_1 =  " << R_1 << "%" <<endl; 





// Create a legend and add entries
    TLegend *legend = new TLegend(0.5, 0.6, 0.7, 0.8); // Position: (x1, y1, x2, y2)
    legend->SetHeader("Gaussian Fit Parameters","C"); // Optionally add a header
    legend->AddEntry((TObject*)0, Form("#scale[0.8]{N_{1}} = %.2f ", Counts), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{#bar{X}_{1}} = %.2f #pm %.2f", mean, meanError), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{#sigma_{x1}} = %.2f #pm %.2f", rms, sigmaError), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{#frac{#sigma_{x1}}{#sqrt{N_{1}}}} = %.2f ", std_error), "");
legend->AddEntry((TObject*)0, "#scale[0.8]{E_{#gamma}} = 661.657 #pm 0.0127  keV", "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{FWHM_{1}} = %.2f", FWHM_1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{R_{1} = %.2f%%}", R_1), "");

    
    
 // Add the legend to the canvas
legend->Draw();



};


void Cs_137() {

Cs_137_Resolution("germaniumdet_cs137.xy", "Cs_137_histo");


}




