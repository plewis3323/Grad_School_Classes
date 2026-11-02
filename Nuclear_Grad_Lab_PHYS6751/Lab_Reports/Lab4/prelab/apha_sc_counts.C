#include <iostream>
#include <cmath>

const double pi = 3.141592653589793;
const double M_alpha = 4.0;    // Mass of alpha particle (u)
const double M_Au = 197.0;     // Mass of Gold (u)
const double Z_alpha = 2.0;    // Atomic number of alpha particle
const double Z_Au = 79.0;      // Atomic number of Gold
const double E0 = 4.5;         // Beam energy in MeV
const double theta = 60.0;     // Scattering angle in degrees
const double n_t = 8e17;       // Target number density (atoms/cm^2)
const double A = 1e-2;         // Detector area in cm^2
const double r = 10.0;         // Distance to detector in cm
const double dOmega = A / (r * r); // Solid angle in sr

// Rutherford differential cross-section at 60 degrees
double RutherfordCrossSection(double theta, double E0) {
    double theta_rad = theta * pi / 180.0; // Convert to radians

    // Calculate the Rutherford differential cross-section in mb/sr
    double factor = 1.296; // mb * MeV^2 / sr
    double Z_ratio = (Z_alpha * Z_Au) / E0;
    double sin_term = 1.0 / pow(sin(theta_rad / 2), 4);
    double mass_term = 2.0 * pow(M_alpha / M_Au, 2);

    // Differential cross-section
    double d_sigma_d_Omega = factor * pow(Z_ratio, 2) * (sin_term - mass_term);
    return d_sigma_d_Omega; // in mb/sr
}

void CalculateDetectedAlphas() {
    // Constants
    double I = 1e-9; // Beam current in A
    double q_alpha = 2 * 1.6e-19; // Charge of alpha in C
    double time = 60.0; // Time in seconds

    // Total number of incident alphas
    double N_incident = (I / q_alpha) * time;
    std::cout << "Total incident alphas: " << N_incident << std::endl;

    // Differential cross-section in mb/sr
    double d_sigma_d_Omega = RutherfordCrossSection(theta, E0);
    std::cout << "Differential cross-section at 60 degrees: " << d_sigma_d_Omega << " mb/sr" << std::endl;

    // Convert mb to cm^2 (1 mb = 1e-27 cm^2)
    d_sigma_d_Omega *= 1e-27;

    // Calculate detected alphas
    double N_detected = N_incident * n_t * 1.0 * d_sigma_d_Omega * dOmega;
    std::cout << "Number of alphas detected: " << N_detected << std::endl;
}















