#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF2.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
using namespace std;

// ================== Constants ==================
double hbar_c = 197.327; // MeV-fm 
double e_sq   = 1.44;    // MeV-fm 
double r_o    = 1.2;     // fm   (keep value as you had; functions unchanged)
double u      = 931.494; // MeV/c^2 
double w_alpha= 1E-2;   // preformation factor
double c      = 3E8;     // m/s
double pi     = M_PI;    // pi 

// =============== Your Functions =================

// Coulomb depth function
double V_C(double Z_alpha, double Z_d, double R){
    double V = ((Z_alpha*Z_d*e_sq)/(R)); 
    return V; // MeV 
} 

// Reduced mass 
double mu(double m_alpha, double m_d) { 
    double U = ((m_alpha*m_d)/(m_alpha + m_d)); 
    return U; // u then later MeV/c^2
} 

// Average Radius (returns fm with r_o in fm)
double R_avg(double A_alpha, double A_d) { 
    double R_1 = r_o*(pow(A_alpha, 1.0/3.0) + pow(A_d, 1.0/3.0)); 
    return R_1; 
} 

// Assault frequency (expects R in fm; converts to meters internally with *1E-15)
double f(double V_0, double Q_alpha, double mu_1, double R_2){ 
    double f_ass = c*sqrt((2.0*(V_0 + Q_alpha))/(mu_1*u))*(1.0/(2*R_2*1E-15)); 
    return f_ass; 
} 

// Tunneling function 
double T(double Z_alpha, double Z_d, double mu_2, double Q_alpha_2, double R_3){ 
    double P_1 = ((-2.0*pi*e_sq)/(hbar_c)); 
    double P_2 = (Z_alpha*Z_d)*sqrt((mu_2*u)/(2.0*Q_alpha_2)); 
    double P_3 = (1.0 - (4.0/pi)*sqrt(R_3/((Z_alpha*Z_d*e_sq)/(Q_alpha_2)))); 
    double Tun = exp(P_1 * P_2 * P_3); 
    return Tun; 
}

// =============== Half-life driver ===============
void half_life(){ 
    // number of elements 
    int n = 5; 

    // Proton number alpha 
    double Z_alpha_T = 2.0;

    // Parent isotopes 
    vector<string> isotopes  = {"U-235", "Pa-231", "Ac-227", "Fr-223", "At-219"};

    // Daughter isotopes 
    vector<string> daughters = {"Th-231","Ac-227","Fr-223","At-219","Bi-215"};

    // Q-values (MeV)
    vector<double> Q_vals = {
        4.678,  // U-235  -> Th-231
        5.150,  // Pa-231 -> Ac-227
        5.042,  // Ac-227 -> Fr-223  
        5.561,  // Fr-223 -> At-219  
        6.342   // At-219 -> Bi-215
    };

    // Daughter Z and A
    vector<double> Z_daughters_A = {90.0, 89.0, 87.0, 85.0, 83.0};
    vector<double> A_daughters   = {231.0, 227.0, 223.0, 219.0, 215.0};

    // Alpha mass (u) and A
    double m_alpha_A = 4.001506179127; 
    double A_alpha_A = 4.0;

    // Daughter nuclear masses (u)
    vector<double> m_daughters_u = {
        231.036304, // Th-231
        227.027752, // Ac-227
        223.019736, // Fr-223
        219.018342, // At-219
        215.001769  // Bi-215
    };

    // formatting
    cout.setf(std::ios::scientific);
    cout << std::setprecision(6);

    // loop over all decays (compact, readable output)
    for (int i = 0; i < n; ++i) {
        const string& Pname = isotopes[i];
        const string& Dname = daughters[i];
        double Qalpha = Q_vals[i];
        double Zd     = Z_daughters_A[i];
        double Ad     = A_daughters[i];
        double md_u   = m_daughters_u[i];

        // Radius: R_avg returns fm (since r_o is fm). Also show meters for readability.
        double R_fm = R_avg(A_alpha_A, Ad);
        double R_m  = R_fm * 1e-15;

        // V0 from Coulomb at contact (needs fm)
        double V0 = V_C(Z_alpha_T, Zd, R_fm);

        // reduced mass (u)
        double mu_u = mu(m_alpha_A, md_u);

        // assault frequency uses your function (R in fm)
        double f_ass = f(V0, Qalpha, mu_u, R_fm);

        // tunneling uses fm
        double P_tun = T(Z_alpha_T, Zd, mu_u, Qalpha, R_fm);

        // decay constant & half-life
        double lambda = w_alpha * f_ass * P_tun; // s^-1
        const double ln2 = 0.6931471805599453;
        double T_sec  = ln2 / std::max(lambda, 1e-300);
        double T_year = T_sec / (3600.0 * 24.0 * 365.25);

        // -------- compact, readable printout --------
        cout << "\n---------------------------------------------\n";
        cout << "Decay: " << Pname << " \u2192 " << Dname << "\n";
        cout << "Q = " << Qalpha << " MeV   V0 = " << V0 << " MeV\n";
        cout << "R = " << R_fm << " fm  (" << R_m << " m)   \u03BC = " << mu_u << " u\n";
        cout << "f = " << f_ass << " s^-1   P = " << P_tun << "\n";
        cout << "λ = " << lambda << " s^-1\n";
        cout << "T1/2 = " << T_sec << " s  (" << T_year << " y)"
             << "   log10(T1/2) = " << log10(T_sec) << "\n";
    }

    cout << "\n=============================================\n";
    cout << "Done.\n";
}












































































































