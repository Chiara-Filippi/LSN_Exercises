/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"
#include "Hamiltonian.h"
#include "SimulatedAnnealing.h"

using namespace std;

int main(){
    double mi_old, sigma_old;
    int nblk_MC, nstep_MC;
    double delta_MC, alpha, T, Tmin;
    vector <double> energy = {0., 0.};
    
    int Tstep = 0;
    double rate = 0.;
    
    //Input
    ifstream ReadInput;
    ReadInput.open("input.in");
    ReadInput >> mi_old;
    ReadInput >> sigma_old;
    ReadInput >> delta_MC;     //energy perturbation for Move() method 
    ReadInput >> nblk_MC;
    ReadInput >> nstep_MC;

    ReadInput >> alpha;     //cooling rate
    ReadInput >> T;
    ReadInput >> Tmin;

    cout << "SA algorithm" << endl;
    cout << "Delta_MC = " << delta_MC << endl;
    cout << "Number of blocks in internal metropolis = " << nblk_MC << endl;
    cout << "Number of steps in each block = " << nstep_MC << endl;

    cout << "Cooling rate alpha = " << alpha << endl;
    cout << "Initial temperature = " << T << endl;
    cout << "Minimum temperature = " << Tmin << endl;
    
    Hamiltonian H(mi_old, sigma_old, delta_MC, nblk_MC, nstep_MC);
    SimulatedAnnealing SA(T, Tmin, alpha);
    Random rnd;
    RandomInitializer(rnd);

    ofstream SA_Parameters;
    ofstream SA_Energy;
    SA_Parameters.open("SA_Parameters");
    SA_Energy.open("SA_Energy");

    while(T > Tmin) {
        rate = SA.Cooling(H, T, SA_Parameters, SA_Energy, rnd, Tstep);
        Tstep ++;
        T = alpha*T; //Temperatura diminuita in base alla legge di raffreddamento
    }

    SA_Energy.close();
    SA_Parameters.close();
    
    cout << "\nTemperature steps:\t" << Tstep << endl;
    
    cout << "\nAcceptance rate:\t" << rate << endl;

    cout << "\nBest values:\t" << H.GetMean() << setw(18) << H.GetSigma() << endl;

    energy = H.Eigenvalue(1);

    cout << "Best energy:\t" << energy[0] << " +/- " <<  energy[1] << endl;

    H.PsiSquareHisto(100, 3, 100000);   //Generato l'istogramma della funzione d'onda
    return 0;
}