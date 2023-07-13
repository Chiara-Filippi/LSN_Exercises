/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Map__
#define __Map__

#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"

using namespace std;

//Classe che genera le posizioni delle città da visitare, su una circonferenza o su un quadrato
class Map {

    public: 

    Map();
    
    Map(int, int, Random&);

    ~Map();

    double GetDistance(int, int);

    int GetNcity();

    vector<double> GetPosition();

    void PrintPosition(int);

    private:
    int m_ncity; //Numero di città
    vector<double> m_position; //Vettore di posizioni delle città
    vector<vector<double>> m_distances; //Matrice delle distanze, vettore di vettori che per ogni città immagazzina la distanza da tutte le altre
};


#endif // __Map__