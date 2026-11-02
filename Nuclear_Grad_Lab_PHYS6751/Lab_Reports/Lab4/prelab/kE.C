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
#include <TLegend.h>
#include <TLine.h>
#include <cmath>
using namespace std; 


void AlphaScattering() {
    // Masses of atoms in atomic mass units (u)
    const double m_alpha = 4.0;   // Mass of alpha particle in u
    const double M_Au = 197.0;    // Gold
    const double M_Ag = 108.0;    // Silver
    const double M_Cu = 63.5;     // Copper

    // Initial alpha kinetic energies in MeV
    double E_alpha_initial[2] = {2.0, 6.0}; // 2 MeV and 6 MeV

    // Scattering angles in degrees
    double angles[2] = {45.0, 135.0}; // 45 degrees and 135 degrees

    // Mass array for target atoms
    double masses[3] = {M_Au, M_Ag, M_Cu};
    const char* materials[3] = {"Gold", "Silver", "Copper"};

    // Loop over all combinations
    for (int i = 0; i < 3; i++) {         // Loop over materials
        for (int j = 0; j < 2; j++) {     // Loop over initial energies
            for (int k = 0; k < 2; k++) { // Loop over scattering angles
                double M = masses[i];       // Target mass
                double E_alpha = E_alpha_initial[j]; // Initial energy
                double theta = angles[k] * M_PI / 180.0; // Angle in radians

                // Calculate the kinetic energy of the recoiling alpha
                double E_alpha_recoil = E_alpha*pow((m_alpha*cos(theta) + sqrt(M*M - m_alpha*m_alpha*sin(theta)*sin(theta)))/(m_alpha + M),2);
                // Output the result
                cout << "Material: " << materials[i] 
                          << ", Initial Energy: " << E_alpha << " MeV, "
                          << "Scattering Angle: " << angles[k] << " degrees -> "
                          << "Recoil Energy: " << E_alpha_recoil << " MeV" << endl;
            }
        }
    }
}


























