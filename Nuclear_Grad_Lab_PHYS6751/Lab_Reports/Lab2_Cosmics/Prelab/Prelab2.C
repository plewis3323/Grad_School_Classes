#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TAxis.h>
#include <TMath.h>
#include <iostream>

void Prelab2() {
    // Define constants
    const int num_points = 10; // Number of angle steps (0 to 90 degrees in steps of 10)
    double angles[num_points]; // Angles in degrees
    double cos2_angles[num_points]; // cos^2(theta) values
    double total_rates[num_points]; // Total rates (true rate + random coincidences)
    double errors[num_points]; // Error in total rates (sqrt of rate)
    const double random_rate = 500.0; // Random coincidence rate (constant)
    const double true_rate_vertical = 1200.0; // True muon rate when telescope is vertical

    // Compute rates at each angle (in steps of 10 degrees)
    for (int i = 0; i < num_points; i++) {
        double theta_deg = i * 10.0; // Angle in degrees
        double theta_rad = TMath::DegToRad() * theta_deg; // Convert to radians for calculation

        // Using degrees for cos²(theta)
        double cos_theta = TMath::Cos(theta_rad); // cos(theta)
        double cos2_theta = cos_theta * cos_theta; // cos^2(theta)
        
        // True rate at this angle
        double true_rate = true_rate_vertical * cos2_theta;

        // Total rate (true + random)
        double total_rate = true_rate + random_rate;

        // Set values
        angles[i] = theta_deg;
        cos2_angles[i] = cos2_theta;
        total_rates[i] = total_rate;
        errors[i] = TMath::Sqrt(total_rate); // Poisson error: sqrt of total rate
    }

    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Muon Rate Distribution", 800, 600);
    c1->Divide(2,1); // Divide canvas into 2 parts for two plots

    // Graph for muon rate vs angle (in degrees)
    c1->cd(1);
    TGraphErrors *gr1 = new TGraphErrors(num_points, angles, total_rates, 0, errors);
    gr1->SetTitle("Total Muon Rate\t vs\t \\theta;\\theta (degrees);\tTotal Rate (muons per second)");
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(kBlue);
    gr1->GetYaxis()->SetTitle("Total Rate (muons per second)");  // Set Y-axis label with units
    gr1->GetXaxis()->SetTitle("\\theta (degrees)");  // Theta for X-axis (in degrees)
    gr1->Draw("AP");

    // Graph for muon rate vs cos^2(theta)
    c1->cd(2);
    TGraphErrors *gr2 = new TGraphErrors(num_points, cos2_angles, total_rates, 0, errors);
    gr2->SetTitle("Total Muon Rate\t vs\t cos^{2}(\\theta);cos^{2}(\\theta);Total Rate (muons per second)");
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerColor(kRed);
    gr2->GetYaxis()->SetTitle("Total Rate (muons per second)");  // Set Y-axis label with units
    gr2->GetXaxis()->SetTitle("cos^{2}(\\theta)");  // cos^2(theta) for X-axis
    gr2->Draw("AP");

    // Update canvas to show plots
    c1->Update();







