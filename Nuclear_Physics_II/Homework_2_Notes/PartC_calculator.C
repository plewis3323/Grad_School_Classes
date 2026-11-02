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
#include <string>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <cmath>
using namespace std; 



//Eta Calculator Code 

//theta function
double theta(double p_t, double m, double y) 
{

double Num = sqrt(pow(m,2) + pow(p_t,2))*sinh(y); 
double Den = sqrt((pow(m,2) + pow(p_t,2))*pow(sinh(y), 2) + pow(p_t,2));
double theta = acos(Num/Den);

return theta;

}





void Rapidity_Calc()

{

vector <double> y = {1, 2, 3}; 
double m_p = 0.938; // GeV 
double p_t = 0.5; // GeV
vector <double> theta_args = {theta(p_t, m_p, y[0]), theta(p_t, m_p, y[1]), theta(p_t, m_p, y[2])}; 




for (int i = 0; i < 3; i++) 
{
    double eta = -log(tan(theta_args[i]/2.0));
    cout << " y = " << y[i] << "  eta = " << eta << "  eta - y = " << eta - y[i] << endl;  
}







}























































































