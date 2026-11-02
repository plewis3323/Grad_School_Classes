#include <iostream>
#include <cmath>

const double pi = 3.141592653589793;
const double M_alpha = 4.0;    // Mass of alpha particle (u)
const double Z_alpha = 2.0;    // Atomic number of alpha particle
const double E0 = 4.5;         // Beam energy in MeV
const double theta = 60.0;     // Scattering angle in degrees
const double N0 = 1.88e11;     // Total incident alphas (from previous result)
const double dOmega = 1e-4;    // Solid angle (sr)

// Target composition: 80% Gold, 15% Silver, 5% Copper
const double n_Au = 8e17*0.8;
const double n_Ag = 8e17*0.15;
const double n_Cu = 8e17*0.05;

// Atomic numbers and masses
const double Z_Au = 79.0, M_Au = 197.0;
const double Z_Ag = 47.0, M_Ag = 108.0;
const double Z_Cu = 29.0, M_Cu = 63.5;

// Differential cross-section function
double DifferentialCrossSection(double Z_target, double M_target, double theta, double E0) {
    double theta_rad = theta * pi / 180.0; // Convert to radians
    double factor = 1.296;                 // mb * MeV^2 / sr
    double Z_ratio = (Z_alpha * Z_target) / E0;
    double sin_term = 1.0 / pow(sin(theta_rad / 2), 4);
    double mass_term = 2.0 * pow(M_alpha / M_target, 2);

    // Calculate the Rutherford differential cross-section in mb/sr
    double d_sigma_d_Omega = factor * pow(Z_ratio, 2) * (sin_term - mass_term);
    return d_sigma_d_Omega; // in mb/sr
}

void CalculateDetectedAlphasMixture() {
    // Calculate differential cross-section for each element
    double d_sigma_d_Omega_Au = DifferentialCrossSection(Z_Au, M_Au, theta, E0);
    double d_sigma_d_Omega_Ag = DifferentialCrossSection(Z_Ag, M_Ag, theta, E0);
    double d_sigma_d_Omega_Cu = DifferentialCrossSection(Z_Cu, M_Cu, theta, E0);

    // Convert mb/sr to cm^2/sr (1 mb = 1e-27 cm^2)
    d_sigma_d_Omega_Au *= 1e-27;
    d_sigma_d_Omega_Ag *= 1e-27;
    d_sigma_d_Omega_Cu *= 1e-27;

    // Calculate the number of detected alphas
    double N_s = N0 * dOmega * (d_sigma_d_Omega_Au * n_Au + d_sigma_d_Omega_Ag * n_Ag + d_sigma_d_Omega_Cu * n_Cu);

    // Output results
    std::cout << "Differential cross-section at 60 degrees (mb/sr):" << std::endl;
    std::cout << "  Gold: " << d_sigma_d_Omega_Au / 1e-27 << " mb/sr" << std::endl;
    std::cout << "  Silver: " << d_sigma_d_Omega_Ag / 1e-27 << " mb/sr" << std::endl;
    std::cout << "  Copper: " << d_sigma_d_Omega_Cu / 1e-27 << " mb/sr" << std::endl;
    std::cout << "Number of alphas detected: " << N_s << std::endl;
}









