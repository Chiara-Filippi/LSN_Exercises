/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Population__
#define __Population__

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"
#include "Map.h"
#include "Route.h"

using namespace std;

void RandomInitializer (Random&, int);
double Error(double, double, int);

class Population{

    public:

    Population(Random&, int, int, Map&);

    ~Population ();

    //void ChangeMap(int, int);

    void NextGen();

    int Selection();

    void Mutation();

    void Crossover (int, int);

    void Sorting();

    double RouteLength(int);

    void PrintBestRoute();

    double MeanLength();

    int GetPopulation();

    Route GetBestPopulation();

    void SetIndividual(Route&, int);

    private:

    int m_npopulation;
    vector<Route> m_pop; //vettore di percorsi
    Map m_map;

    Random rnd;
};

#endif // __Population__