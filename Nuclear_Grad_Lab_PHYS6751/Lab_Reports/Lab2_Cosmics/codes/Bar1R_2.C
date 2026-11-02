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


void Threshold(TCanvas* Bar) {
Bar->cd(); 
int t = 20; //sec
//Number of entries for each
Double_t Voltages[15] = {51.4, 60.4, 69.8, 81.4, 90.2, 100.1, 110.6, 120.1, 130.5, 139.1, 150.8, 20.0, 30.5, 41.0, 49.8};
Double_t Counts[15] = {12453, 13307, 12400, 11014, 11097, 9562, 8238, 7441, 6374, 5624, 4880, 306259, 62981, 41024, 17676};


vector <double> C_rates(21); 
for(int i=0; i<15; i++)
{
   C_rates[i] = Counts[i]/t; 
   cout << "The Count rates are: " << C_rates[i] << endl; 
}

vector<double> CountR_errors(21);
for(int j=0; j<15; j++)
{

     CountR_errors[j] = sqrt(C_rates[j]); 
     cout << "The Counts are: " << Counts[j] << endl;
     cout << "The Voltages are: " << Voltages[j] << endl;
     cout << "The Count errors are: " << CountR_errors[j]<< endl; 

}




int n = 15;


 // Vector to store the elements with the smallest differences
    vector<pair<Double_t, int>> smallestElements;

    // Iterate through the elements, excluding the first and last
    for (int p = 1; p < n - 1; ++p) {
        Double_t leftDiff = abs(Counts[p] - Counts[p - 1]);   // Absolute difference with left neighbor
        Double_t rightDiff = abs(Counts[p] - Counts[p + 1]);  // Absolute difference with right neighbor

        // If both left and right differences are considered "small," store the element
        // We will store elements in the format (combinedDifference, index)
        Double_t combinedDiff = leftDiff + rightDiff;
        smallestElements.push_back(make_pair(combinedDiff, p));
    }

    // Sort the vector based on the combined differences
    sort(smallestElements.begin(), smallestElements.end());

    // Print the top 4 elements with the smallest combined left and right differences
    cout << "The 4 smallest elements based on left and right neighbor differences:" << endl;
    for (int i = 0; i < min(4, (int)smallestElements.size()); ++i) {
        int index = smallestElements[i].second;
        cout << "Element " << Counts[index] << " at index " << index 
             << " with combined difference of " << smallestElements[i].first << endl;
    }


// Create TGraphErrors
// Convert std::vector<double> to arrays for TGraphErrors
TGraphErrors* graph = new TGraphErrors(n, Voltages, C_rates.data(), 0, CountR_errors.data());
graph->SetTitle("Bar#1 Right: Threshold");
graph->Draw("AP");
graph->SetMarkerStyle(8);
graph->SetMarkerSize(2.0);  // Set marker size
graph->SetMarkerColor(kRed);  // Set marker color
graph->SetLineWidth(5);     // Set line width for error bars


 // Fit the graph with a cubic function
TF1 *fitFunc1 = new TF1("fitFunc1", " pol3 ", 0, 200);  
fitFunc1->SetLineColor(5);
graph->Fit(fitFunc1, "R");


 // Fit the graph with a cubic function
TF1 *fitFunc3 = new TF1("fitFunc3", " [0] ", 0, 230);  
fitFunc3->SetLineColor(6);
graph->Fit(fitFunc3, "R+");


    // Draw a vertical line at 85.8 mV (threshold voltage)
    TLine *thresholdLine = new TLine(85.8, 0, 85.8, *max_element(C_rates.begin(), C_rates.end()));
    thresholdLine->SetLineColor(kGreen);
    thresholdLine->SetLineStyle(2);  // Dashed line
    thresholdLine->SetLineWidth(2);
    thresholdLine->Draw("SAME");









// Center the X-axis label by adjusting the offset and title size
graph->GetXaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetXaxis()->SetTitleSize(0.05);  // Adjust the size if necessary
// Center the Y-axis label by adjusting the offset and title size
graph->GetYaxis()->SetTitleOffset(0.95); // Smaller offset moves the title closer to the plot
graph->GetYaxis()->SetTitleSize(0.05);  // Adjust the size if necessary
// Set the X-axis and Y-axis labels
graph->GetXaxis()->SetTitle("mV");
graph->GetYaxis()->SetTitle("Counts/sec");





        

        
 // Add a legend to the canvas
    TLegend *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
    legend->AddEntry(graph, "Data points", "p");
    legend->AddEntry(fitFunc1, "Pol3 Fit ", "l");
    legend->AddEntry(fitFunc3, "Linear Fit ", "l");
    legend->AddEntry(thresholdLine, "Threshold Voltage at 85.8 mV", "l");
    legend->Draw();
    


}



void Run1() {



TCanvas *c1 = new TCanvas("c1"); 
Threshold(c1);
TFile *Bars = new TFile("Bar1R.root", "RECREATE"); 
c1->Write();
Bars->Close(); 
    
}



