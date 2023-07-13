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
#include <unistd.h>
#include "random.h"
#include "SimulatedAnnealing.h"


using namespace std;

//Costruttore
SimulatedAnnealing :: SimulatedAnnealing(double T_in, double T_fin, double alpha){

    m_T_in = T_in; //temperatura iniziale
    m_T_fin = T_fin; //temperatura finale
    m_alpha = alpha;
    
    return;
}

//Distruttore
SimulatedAnnealing :: ~SimulatedAnnealing() {}

double SimulatedAnnealing :: Cooling(Hamiltonian &H, double T, ofstream &Parameters, ofstream&Energy, Random&rnd, int Tstep) {
    double p, A;
    vector<double> energy = {0., 0.};

    double mi_old = H.GetMean();
    double sigma_old = H.GetSigma();
    vector <double> energy_old = H.Eigenvalue(0);

    double mi = mi_old + 0.1 * rnd.Rannyu(-1., 1.); //Generata una nuova mi ottenuta perturbando di poco l'originale
    double sigma = sigma_old + 0.1 * rnd.Rannyu(-1., 1.); //Generata una nuova sigma ottenuta perturbando di poco l'originale

    H.SetParameters(mi, sigma); 

    vector <double> energy_new = H.Eigenvalue(0); //Calcolato il nuovo autovalore
    p = exp( - 1./T * ( energy_new[0] - energy_old[0] )); //Calcolata la probabilità di accettazione con il peso di Boltzmann
    A = min(1., p);

    double r = rnd.Rannyu();
    if( A == 1. || r <= A ){ //Accettata la mossa in automatico se A è 1, poiché significa che energy_new < energy_old, oppure se A soddisfa la condizione di Metropolis
        energy = energy_new;
        H.SetParameters(mi, sigma);
        accepted ++;
    } 
    else { //Rigettata la mossa
        energy = energy_old;
        H.SetParameters(mi_old, sigma_old);
    }
    
    attempted ++;

    Parameters << Tstep << setw(18) << H.GetMean() << setw(18) << H.GetSigma() << endl;
    Energy << Tstep << setw(18) << energy[0] << setw(18) << energy[1] << endl;

    return (double)accepted/(double)attempted;

}
