

/* 


Q_4.  Determine the energy resolution of the detector. Does the energy resolution depend on gamma-ray energy? If so, describe the energy dependence of the energy resolution.


*/ 





/* 


                       Co-60   Energy Resolutions.
                       

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




void Co_Res_Resolution(TString filename, TString histname){

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
TCanvas *c1 = new TCanvas("c1", "Co_60 spectrum", 800, 600);

// Drawing the histogram on the canvas
myhist->Draw();
    
// Zoom in on the x-axis
myhist->GetXaxis()->SetRangeUser(0, 6000);  


 // Perform a Gaussian Fit #1
    TF1 *gausFit1 = new TF1("gausFit1", "gaus", 3510, 3550);
    myhist->Fit(gausFit1, "R");  // "R" means fit in the specified range
    // Draw the fit
    gausFit1->SetLineColor(kRed);
    gausFit1->Draw("SAME");
    
    
    
// Perform a Gaussian Fit #2 
TF1 *gausFit2 = new TF1("gausFit2", "gaus", 4000, 4040); 
myhist->Fit(gausFit2, "R"); 
//Draw the fit 
gausFit2->SetLineColor(kRed);
gausFit2->Draw("SAME");


//Finding the Number of Counnts from Bins 3510-3550: Peak #1
double Counts_p1 = myhist->Integral(3510,3550);
cout << "The number of Counts:  N = " << Counts_p1 << endl; 

//Finding the Number of Counnts from Bins 4000-4040: Peak #2
double Counts_p2 = myhist->Integral(4000,4040);
cout << "The number of Counts:  N = " << Counts_p2 << endl; 


//Retrieve the parameters for peak #1 with bins 3510-3550
    Double_t mean1 = gausFit1->GetParameter(1);      // Mean (μ) of the Gaussian Peak#1
    Double_t rms1 = gausFit1->GetParameter(2);     // Sigma (σ), the standard deviation

    // Retrieve the errors associated with each parameter
    Double_t meanError1 = gausFit1->GetParError(1);      // Error on the mean
    Double_t sigmaError1 = gausFit1->GetParError(2);     // Error on the sigma

    // Output the parameters with their errors
    cout << "Mean: " << mean1 << " ± " << meanError1 << endl;
    cout << "Sigma (RMS): " << rms1 << " ± " << sigmaError1 << endl;



//Retrieve the parameters for peak #2 with bins 4000-4040
    Double_t mean2 = gausFit2->GetParameter(1);      // Mean (μ) of the Gaussian Peak#1
    Double_t rms2 = gausFit2->GetParameter(2);     // Sigma (σ), the standard deviation

    // Retrieve the errors associated with each parameter
    Double_t meanError2 = gausFit2->GetParError(1);      // Error on the mean
    Double_t sigmaError2 = gausFit2->GetParError(2);     // Error on the sigma

    // Output the parameters with their errors
    cout << "Mean: " << mean2<< " ± " << meanError2 << endl;
    cout << "Sigma (RMS): " << rms2 << " ± " << sigmaError2 << endl;
    
    
//Standard error of the mean for Peak #1
    double std_error1 =  rms1/(sqrt(Counts_p1));
    cout << "the standard error of the mean of Peak #1: " << std_error1 << endl; 
    
    
    
    
//Standard error of the mean for Peak #2
    double std_error2 =  rms1/(sqrt(Counts_p2));
    cout << "the standard error of the mean of Peak #2: " << std_error2 << endl;
    

// Known energies in (keV) and corresponding channel numbers
Double_t Co_Energies[2] = {1173.21, 1332.43};
Double_t Co_Channel_N[2]  = {3531.45, 4016.93};



// Slope and intercept
double a = 0.327956;
    
    
    
// FWHM = 2.355 * RMS    
// FWHM of Peak #1 where  mean channel = 3531.45
double FWHM_1 = 2.355 * rms1*a;
cout << "the full width half max of peak #1; FWHM =   " << FWHM_1 << endl; 


// FWHM of Peak #2 where  mean channel = 4016.93
double FWHM_2 = 2.355 * rms2*a;
cout << "the full width half max of peak #2; FWHM =   " << FWHM_2 << endl; 




// Resolution: R = FWHM/(E_gamma) * 100 
// Resolution of Peak #1 where channel = 3531.45
double R_1 = (FWHM_1/Co_Energies[0]) * 100; 
cout << " R_1 =  " << R_1 << "%" <<endl; 


// Resolution of Peak #2 where channel = 4016.93
double R_2 = (FWHM_2/Co_Energies[1]) * 100; 
cout << " R_2 =  " << R_2 << "%" <<endl; 





// Create a legend and add entries
    TLegend *legend = new TLegend(0.5, 0.6, 0.7, 0.8); // Position: (x1, y1, x2, y2)
    legend->SetHeader("Gaussian Fit Parameters","C"); // Optionally add a header
    legend->AddEntry((TObject*)0, Form("#scale[0.8]{N_{1}} = %.2f ", Counts_p1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{N_{2}} = %.2f ", Counts_p2), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{#bar{X}_{1}} = %.2f #pm %.2f", mean1, meanError1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{#sigma_{x1}} = %.2f #pm %.2f", rms1, sigmaError1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.8]{#bar{X}_{2}} = %.2f #pm %.2f", mean2, meanError2), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{#sigma_{x2}} = %.2f #pm %.2f", rms2, sigmaError2), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{#frac{#sigma_{x1}}{#sqrt{N_{1}}}} = %.2f ", std_error1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{#frac{#sigma_{x2}}{#sqrt{N_{2}}}} = %.2f ", std_error2), "");
legend->AddEntry((TObject*)0, "#scale[0.8]{E_{#gamma}} = 1173.21 #pm 0.0205 keV", "");
legend->AddEntry((TObject*)0, "#scale[0.8]{E_{#gamma}} = 1332.43 #pm 0.0259 keV", "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{FWHM_{1}} = %.2f", FWHM_1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{FWHM_{1}} = %.2f", FWHM_2), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{R_{1} = %.2f%%}", R_1), "");
legend->AddEntry((TObject*)0, Form("#scale[0.6]{R_{2} = %.2f%%}", R_2), "");

    
    
 // Add the legend to the canvas
legend->Draw();



};


void Co_60() {

Co_Res_Resolution("germaniumdet_co60.xy", "Co_60_histo");


}


















