
#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"

void E2b() {
    TCanvas *c1 = new TCanvas("c1", "Jacobian", 800, 600);

    double pT = 0.5;

    //Neutral Pion 
    TF1 *fPion = new TF1("fPion",
        "[1]*cosh(x) / sqrt([0]*[0]+[1]*[1] + [1]*[1]*sinh(x)*sinh(x))",
        -5, 5);
    fPion->SetParameters(0.135, pT);
    fPion->SetLineColor(kBlue);
    fPion->SetLineWidth(2);
    fPion->SetMinimum(0.0);
    fPion->SetMaximum(1.1);
    fPion->SetTitle("dy/d#eta = #beta;#eta;dy/d#eta");
    fPion->Draw();

    // Kaon
    TF1 *fKaon = new TF1("fKaon",
        "[1]*cosh(x) / sqrt([0]*[0]+[1]*[1] + [1]*[1]*sinh(x)*sinh(x))",
        -5, 5);
    fKaon->SetParameters(0.494, pT);
    fKaon->SetLineColor(kRed);
    fKaon->SetLineWidth(2);
    fKaon->Draw("SAME");

    // Proton
    TF1 *fProton = new TF1("fProton",
        "[1]*cosh(x) / sqrt([0]*[0]+[1]*[1] + [1]*[1]*sinh(x)*sinh(x))",
        -5, 5);
    fProton->SetParameters(0.938, pT);
    fProton->SetLineColor(kGreen+2);
    fProton->SetLineWidth(2);
    fProton->Draw("SAME");

    // Legend
    TLegend *leg = new TLegend(0.6, 0.15, 0.85, 0.35);
    leg->AddEntry(fPion,   "#pi^{0} (m=0.135)", "l");
    leg->AddEntry(fKaon,   "K^{0} (m=0.494)", "l");
    leg->AddEntry(fProton, "p (m=0.938)", "l");
    leg->Draw();

    c1->SaveAs("E2b.root");
}

































































































































