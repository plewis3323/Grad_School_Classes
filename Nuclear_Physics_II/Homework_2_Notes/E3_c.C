#include <iostream>
#include <TF1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLatex.h>
#include <cmath>

void E3_c()
{
    TCanvas *c1 = new TCanvas("c1", "E3c - Opening Angle Distribution", 800, 600);

    // Constants for 1 GeV pi0
    double m_pi = 0.135;           // GeV
    double E_pi = 1.0;             // GeV
    double gamma = E_pi / m_pi;    // ~7.41
    double beta = sqrt(1.0 - 1.0 / (gamma * gamma));

    // Minimum opening angle: cos(Dtheta_min) = 1 - 2/gamma^2
    double Dtheta_min = acos(1.0 - 2.0 / (gamma * gamma));

    // dN/d(Delta theta) = sin(Dth) / [ beta*gamma^2*(1-cos(Dth))^2 * sqrt(1 - 2/(gamma^2*(1-cos(Dth)))) ]
    // x[0] = Delta theta (radians), p[0] = gamma, p[1] = beta
    TF1 *f1 = new TF1("f1", [](double *x, double *p) {
        double gamma = p[0];
        double beta  = p[1];
        double Dth   = x[0];
        double cosDth = cos(Dth);
        double sinDth = sin(Dth);
        double one_minus_cos = 1.0 - cosDth;

        double arg = 1.0 - 2.0 / (gamma * gamma * one_minus_cos);

        // Below minimum opening angle, distribution is zero
        if (arg <= 0.0) return 0.0;

        return sinDth / (beta * gamma * gamma * one_minus_cos * one_minus_cos * sqrt(arg));
    }, Dtheta_min * 0.95, TMath::Pi(), 2);

    f1->SetParameters(gamma, beta);
    f1->SetNpx(10000);  // smooth curve near the singularity
    f1->SetLineColor(kBlue);
    f1->SetLineWidth(2);
    f1->SetTitle("Opening Angle Distribution for E_{#pi^{0}} = 1 GeV;  #Delta#theta (rad);  #frac{dN}{d(#Delta#theta)}");
    f1->Draw();

    // Mark the minimum opening angle
    TLatex *tex = new TLatex(Dtheta_min + 0.05, f1->GetMaximum() * 0.85,
        Form("#Delta#theta_{min} = %.2f rad = %.1f#circ", Dtheta_min, Dtheta_min * 180.0 / TMath::Pi()));
    tex->SetTextSize(0.035);
    tex->SetTextColor(kRed);
    tex->Draw();

    c1->SaveAs("E3c.root");
}








































