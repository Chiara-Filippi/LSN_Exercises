/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Route__
#define __Route__

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"
#include <algorithm>
#include "Map.h"

using namespace std;

class Route{

    public:

    Route();

    Route(int, Random&, Map&);
    
    ~Route ();

    vector<int> GetRoute();

    void SetRoute(vector<int>);

    int GetCityIndex(int);

    int GetNcity();

    double GetLength() const;

    void CalculateLength(Map&);

    void PairPermutation(Random&);

    void Shift(Random&);
           
    void Permutation(Random&);

    void Inversion(Random&);

    void Print();

    int pbc(int);

    bool operator<(const Route&) const;

    private:

    int m_ncity;            
    vector<int> m_route;    
    double m_length;

};

#endif // __Route__