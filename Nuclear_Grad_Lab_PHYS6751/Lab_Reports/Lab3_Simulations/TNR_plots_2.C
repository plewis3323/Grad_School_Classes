#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TF1.h>
#include <TFile.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <vector>

// Function to read CSV file and return a TGraph pointer
TGraph* ReadCSVtoTGraph(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return nullptr;
    }

    std::vector<double> energies;
    std::vector<double> tnrs;
    std::string line;

    // Read file line by line and skip the header
    bool isHeader = true;
    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        double energy, tnr;
        std::stringstream ss(line);
        char comma;

        // Assuming the format "Beam Energy (MeV), TNR"
        if (ss >> energy >> comma >> tnr) {
            energies.push_back(energy);
            tnrs.push_back(tnr);
        }
    }
    file.close();

    // Create a TGraph from the data
    TGraph* graph = new TGraph(energies.size(), &energies[0], &tnrs[0]);
    graph->GetXaxis()->SetTitle("Beam Energy (MeV)");
    graph->GetYaxis()->SetTitle("TNR");
    return graph;
}

// Main function to plot TNR vs. Beam Energy for each beam type on separate canvases
void plot_tnr_vs_energy() {
    // Directory where the CSV files are located
    TString directory = "/home/plewis/Gradlab/Lab_reports/Lab3_Simulations/"; // Replace with actual path

    // User-defined axis ranges for each source
    // Ion Beam
    double ion_xMin = 1000;
    double ion_xMax = 4000;
    // Proton Beam
    double proton_xMin = 0.0;
    double proton_xMax = 200.0;
    // Gamma Beam
    double gamma_xMin = 0.2;
    double gamma_xMax = 1.1;

    double yMin = 0.0;      // Minimum TNR (common for all)
    double yMax = 1.0;       // Maximum TNR (common for all)

    // Load each CSV file and create a TGraph for each
    TGraph* ionGraph = ReadCSVtoTGraph(std::string(directory + "ion.csv"));
    TGraph* protonGraph = ReadCSVtoTGraph(std::string(directory + "proton.csv"));
    TGraph* gammaGraph = ReadCSVtoTGraph(std::string(directory + "gamma.csv"));

    // Check if the graphs were created successfully
    if (!ionGraph || !protonGraph || !gammaGraph) {
        std::cerr << "Error: Could not load all graphs." << std::endl;
        return;
    }

    // 1. Plot for Ion Beam
    TCanvas* ionCanvas = new TCanvas("ionCanvas", "Ion Beam TNR vs Beam Energy", 800, 600);
    ionGraph->SetMarkerStyle(20);
    ionGraph->SetMarkerColor(kBlue);
    ionGraph->SetLineColor(kBlue);
    ionGraph->Draw("AP");  // "A" for axis, "P" for points, "L" for line
    ionGraph->GetXaxis()->SetLimits(ion_xMin, ion_xMax);  // Set enforced X-axis limits for Ion
    ionGraph->GetYaxis()->SetRangeUser(yMin, yMax);       // Set Y-axis range

    
    TF1 *gausFit = new TF1("gausFit", "gaus", 3000, 3150); 
    ionGraph->Fit(gausFit, "R"); 
    //Draw the fit 
    gausFit->SetLineColor(7);
    gausFit->Draw("SAME");
    ionCanvas->Update();
    ionCanvas->SaveAs("Ion_TNR_vs_BeamEnergy.root");

    // 2. Plot for Proton Beam
    TCanvas* protonCanvas = new TCanvas("protonCanvas", "Proton Beam TNR vs Beam Energy", 800, 600);
    protonGraph->SetMarkerStyle(21);
    protonGraph->SetMarkerColor(kRed + 2);
    protonGraph->SetLineColor(kRed + 2);
    protonGraph->Draw("AP");
    protonGraph->GetXaxis()->SetLimits(proton_xMin, proton_xMax);  // Set enforced X-axis limits for Proton
    protonGraph->GetXaxis()->SetRangeUser(proton_xMin, proton_xMax);  // Also apply SetRangeUser
    protonGraph->GetYaxis()->SetRangeUser(yMin, yMax);                // Set Y-axis range

     TF1 *gausFit2 = new TF1("gausFit2", "gaus", 120, 145); 
     protonGraph->Fit(gausFit2, "R"); 
     //Draw the fit 
     gausFit2->SetLineColor(5);
     gausFit2->Draw("SAME");
     protonCanvas->Update();
     protonCanvas->SaveAs("Proton_TNR_vs_BeamEnergy.root");

    // 3. Plot for Gamma Beam
    TCanvas* gammaCanvas = new TCanvas("gammaCanvas", "Gamma Beam TNR vs Beam Energy", 800, 600);
    gammaGraph->SetMarkerStyle(22);
    gammaGraph->SetMarkerColor(kGreen + 2);
    gammaGraph->SetLineColor(kGreen + 2);
    gammaGraph->Draw("AP");
    gammaGraph->GetXaxis()->SetLimits(gamma_xMin, gamma_xMax);  // Set enforced X-axis limits for Gamma
    gammaGraph->GetYaxis()->SetRangeUser(yMin, yMax);           // Set Y-axis range

    gammaCanvas->Update();
    gammaCanvas->SaveAs("Gamma_TNR_vs_BeamEnergy.root");
}


