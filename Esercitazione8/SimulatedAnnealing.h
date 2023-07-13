/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __SimulatedAnnealing__
#define __SimulatedAnnealing__

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"
#include "Hamiltonian.h"

using namespace std;

class SimulatedAnnealing{

    public: 
    //Costruttore
    SimulatedAnnealing(double, double, double);
    //Distruttore
    ~SimulatedAnnealing();
    double Cooling(Hamiltonian &, double, ofstream &, ofstream&, Random&, int);

    //Class members
    private: 

    //Parameters, observables
    double m_T_in, m_T_fin;
    double m_alpha;
    int attempted = 0, accepted = 0;
};

#endif // __SimulatedAnnealing__