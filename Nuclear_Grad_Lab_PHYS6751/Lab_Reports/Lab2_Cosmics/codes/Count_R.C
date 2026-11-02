#include <iostream> 
#include <vector>
#include <cmath>

using namespace std; 

void CountR() 
{
    int n = 8;
    double t = 30.0;
    
    // Array of counts
    double Counts[8] = {2806.0, 2924.0, 3232.0, 3207.0, 160.0, 159.0, 142.0, 153.0};

    // Vector for uncertainties
    vector<double> U_Counts(n);
    vector<double> Count_R(n);
    vector<double> CountR_Error(n);

    // Calculate uncertainties and count rates
    for (int p = 0; p < n; p++) 
    {
        // Uncertainty is the square root of the count
        U_Counts[p] = sqrt(Counts[p]);
        cout << "The Count Uncertainties: " << U_Counts[p] << endl;

        // Calculate count rate (Counts / time)
        Count_R[p] = Counts[p] / t;
        cout << "The Count Rates: " << Count_R[p] << endl;

        // Calculate the uncertainty of the count rate (Uncertainty of count / time)
        CountR_Error[p] = U_Counts[p] / t;
        cout << "The Count Rate errors: " << CountR_Error[p] << endl;

        // Print the count itself
        cout << "The Counts are: " << Counts[p] << endl;
    }
}



