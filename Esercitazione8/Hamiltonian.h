/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Hamiltonian__
#define __Hamiltonian__

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"

using namespace std;

void RandomInitializer (Random&);
double Error(double, double, int);

class Hamiltonian{

    public: 
    //Costruttore
    Hamiltonian(double, double, double, int, int);
    //Distruttore
    ~Hamiltonian();

    vector<double> Eigenvalue(int);

    void PsiSquareHisto(int, double, int);

    void SetParameters(double, double);

    double GetMean();

    double GetSigma();

    double GetDelta();

    void SetBlockParameters(int, int);

    double Unif();

    void Move();

    double PsiSquare(double);

    void Measure();

    void Reset(int);

    void Accumulate(void);

    vector<double> Averages(int, int);

    //Class members
    private: 

    //Average
    double blk_av, blk_norm, accepted, attempted;
    double glob_av, glob_av2;
    double estimate_energy;
    double err_energy;

    //Simulation
    int m_nstep, m_nblk;
    double m_delta;
    Random rnd;

    //Configuration
    double x;
    double xnew;

    //Parameters, observables
    double m_mi, m_sigma;
    double hbar = 1.;
    double m = 1.;
    double Energy;

};

#endif // __Hamiltonian__