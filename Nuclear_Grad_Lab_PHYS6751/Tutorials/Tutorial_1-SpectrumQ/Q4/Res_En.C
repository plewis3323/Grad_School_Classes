

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TAxis.h>
#include <TLegend.h>

void Energy_Resolution_Plot() {
    // Data points for Energy (keV), Resolution (FWHM), and their errors
    const int n = 5;
    double energy_values[n] = {661.657, 1173.21, 1332.43, 511.006, 1274.53};
    double energy_errors[n] = {0.0127, 0.0205, 0.0259, 0.0156, 0.0239};
    double resolution_values[n] = {2.78, 3.35, 3.31, 4.19, 3.71};
    double resolution_errors[n] = {0.42, 0.29, 0.25, 0.82, 0.29};

    // Create a canvas
    TCanvas *c1 = new TCanvas("c1", "Energy vs Resolution", 800, 600);

    // Create a graph with error bars
    TGraphErrors *graph = new TGraphErrors(n, energy_values, resolution_values, energy_errors, resolution_errors);
    graph->SetTitle("Energy vs Resolution;Energy (keV);Resolution (FWHM)");
    graph->SetMarkerStyle(21);
    graph->SetMarkerColor(kBlue);
    graph->SetLineColor(kBlue);

    // Draw the graph
    graph->Draw("AP");

    // Create a fit function (linear)
    TF1 *fit = new TF1("fit", "[0]*x + [1]", 500, 1400);
    graph->Fit(fit);

    // Customize the plot
    graph->GetXaxis()->SetTitle("Energy (keV)");
    graph->GetYaxis()->SetTitle("Resolution (FWHM)");

    // Draw a legend
    TLegend *legend = new TLegend(0.15, 0.75, 0.45, 0.85);
    legend->AddEntry(graph, "Data with errors", "lep");
    legend->AddEntry(fit, Form("Fit: y = %.2e x + %.2f", fit->GetParameter(0), fit->GetParameter(1)), "l");
    legend->Draw();

    // Update the canvas
    c1->Update();
}

