#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"

const double pi = 3.141592653589793;
const double M_alpha = 4.0;    // Mass of alpha particle (in atomic mass units)
const double M_Au = 197.0;     // Mass of Gold (in atomic mass units)
const double Z_alpha = 2.0;    // Atomic number of alpha particle
const double Z_Au = 79.0;      // Atomic number of Gold

// Rutherford differential cross-section function
double RutherfordCrossSection(double theta, double E0) {
    // Convert angle to radians
    double theta_rad = theta * pi / 180.0;

    // Calculate the Rutherford differential cross-section in mb/sr
    double factor = 1.296; // mb * MeV^2 / sr
    double Z_ratio = (Z_alpha * Z_Au) / E0;
    double sin_term = 1.0 / pow(sin(theta_rad / 2), 4);
    double mass_term = 2.0 * pow(M_alpha / M_Au, 2);

    // Differential cross-section
    double d_sigma_d_Omega = factor * pow(Z_ratio, 2) * (sin_term - mass_term);

    return d_sigma_d_Omega; // Returns cross-section in mb/sr
}

void PlotRutherfordCrossSection() {
    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Rutherford Cross-Section", 1200, 600);
    c1->Divide(2, 1); // Two subplots side-by-side

    // Plot 1: Cross-section vs. beam energy (constant angle)
    c1->cd(1);
    const int nE = 100;
    double energies[nE], cross_sections1[nE];
    double angle = 60.0; // Constant angle of 60 degrees

    // Calculate cross-section for varying beam energy
    for (int i = 0; i < nE; i++) {
        energies[i] = 2.0 + (4.0 * i) / (nE - 1); // From 2 to 6 MeV
        cross_sections1[i] = RutherfordCrossSection(angle, energies[i]);
    }

    TGraph *g1 = new TGraph(nE, energies, cross_sections1);
    g1->SetTitle("Cross-section vs. Beam Energy (Angle = 60 degrees)");
    g1->GetXaxis()->SetTitle("Beam Energy (MeV)");
    g1->GetYaxis()->SetTitle("Cross-section (mb/sr)");
    g1->SetLineColor(kBlue);
    g1->SetLineWidth(2);
    g1->SetMarkerStyle(20);    // Marker style (full circle)
    g1->SetMarkerSize(0.8);    // Marker size
    g1->SetMarkerColor(kBlue); // Marker color
    g1->Draw("APL");           // Draw axis, points, and line

    // Plot 2: Cross-section vs. angle (constant beam energy)
    c1->cd(2);
    const int nTheta = 100;
    double angles[nTheta], cross_sections2[nTheta];
    double beam_energy = 4.5; // Constant energy of 4.5 MeV

    // Calculate cross-section for varying angle
    for (int i = 0; i < nTheta; i++) {
        angles[i] = 25.0 + (130.0 * i) / (nTheta - 1); // From 25 to 155 degrees
        cross_sections2[i] = RutherfordCrossSection(angles[i], beam_energy);
    }

    TGraph *g2 = new TGraph(nTheta, angles, cross_sections2);
    g2->SetTitle("Cross-section vs. Scattering Angle (E = 4.5 MeV)");
    g2->GetXaxis()->SetTitle("Scattering Angle (degrees)");
    g2->GetYaxis()->SetTitle("Cross-section (mb/sr)");
    g2->SetLineColor(kRed);
    g2->SetLineWidth(2);
    g2->SetMarkerStyle(21);    // Marker style (full square)
    g2->SetMarkerSize(0.8);    // Marker size
    g2->SetMarkerColor(kRed);  // Marker color
    g2->Draw("APL");           // Draw axis, points, and line

    // Update canvas
    c1->Update();
}








