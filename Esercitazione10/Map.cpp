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
#include <string>
#include "random.h"
#include "Map.h"


using namespace std;

//Costruttore di default
Map::Map(){}

//Costruttore
Map::Map(int ncity, ifstream& Capitals){
    m_ncity = ncity;
    m_position.resize(2 * m_ncity);

    //Leggiamo le capitali
    for(int i = 0; i<m_ncity; i++) {
        Capitals >> m_position[2*i] >> m_position[2*i + 1];
    }

    //Calcolo delle distanze
    m_distances.resize(m_ncity, vector<double>(m_ncity));

    double x1, x2, y1, y2, distance;

    for (int i = 0; i < m_ncity; i++) {
        x1 = m_position[2*i];
        y1 = m_position[2*i+1];

        for (int j = i; j < m_ncity; j++) { //Immagazziniamo anche gli 0 diagonali
            x2 = m_position[2*j];
            y2 = m_position[2*j+1];
            distance = sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) ); //norma L^1

            //Conserviamo nella matrice delle distanze
            m_distances[i][j] = distance;
            m_distances[j][i] = distance; //Copiamo nell'elemento simmetrico
        }
    }
}

//Distruttore
Map::~Map () {}

double Map :: GetDistance(int i, int j){
    return m_distances[i][j];
}

int Map :: GetNcity(){
    return m_ncity;
}

vector<double> Map :: GetPosition(){
    return m_position;
}

void Map::PrintPosition(){

    ofstream Position;
    Position.open("Map");

    for(int i = 0; i < m_ncity; i++ ){
        Position << m_position[2*i] << setw(12) << m_position[2*i + 1] << endl; //stampate la x e la y della città 
    }

    Position.close();
}