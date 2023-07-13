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
#include "Route.h"
#include <algorithm>

using namespace std;

//Costruttore di default
Route::Route(){}

//Costruttore
Route::Route(int ncity, Random& rnd, Map& map){
    m_ncity = ncity;        //Numero di città
    m_route.resize(m_ncity);  
    
    //Generato un percorso pseudocasuale
    for (int i = 0; i < m_ncity; i++) {
        m_route[i] = i;
    }
    int j;
    for (int i = 1; i < m_ncity; i++) { 
        j = rnd.Rannyu(1, m_ncity);
        swap(m_route[i], m_route[j]); //percorso generato swappando la città di indice i con la città di indice j, estratto pseudocasualmente tra 1 e i. La prima città, di indice 0, non viene toccata
    }

    //Calcolo della lunghezza del percorso
    double L = 0.;
    for(int i = 0; i < m_ncity - 1; i++){
        L += map.GetDistance(m_route[i], m_route[i+1]);
    }
    L += map.GetDistance(m_route[m_ncity - 1], m_route[0]); //sommata la distanza tra città finale e iniziale
    m_length = L;
}
    
//Distruttore
Route::~Route () {}

vector<int> Route::GetRoute(){
    return m_route;
}

void Route::SetRoute(vector<int> route){
    for(int i = 0; i < m_ncity; i++){
        m_route[i] = route[i];
    }
}

int Route::GetCityIndex(int index){
    return m_route[index];
}

int Route::GetNcity(){
    return m_ncity;
}

double Route::GetLength() const{
    return m_length;
}

//Calcola la lunghezza del percorso dopo che questo è stato modificato
void Route::CalculateLength(Map& map){
    double L = 0.;
    for(int i = 0; i < m_ncity - 1; i++){
        L += map.GetDistance(m_route[i], m_route[i+1]);
    }
    L += map.GetDistance(m_route[m_ncity - 1], m_route[0]);
    m_length = L;
}

//Primo esempio di operatore di mutazione genetica: invertite le città nella posizione i e j
void Route::PairPermutation(Random& rnd){
    int i, j;     
    do{
        i = floor(rnd.Rannyu(1, m_ncity)); //Numero pseudocasuale discreto tra 1 e il numero totale di città
        j = floor(rnd.Rannyu(1, m_ncity));
    } while( i == j );
    //Esempio: [1, 2, 3, 4, 5] -> [1, 2, 4, 3, 5], i = 2, j = 3
    swap(m_route[i], m_route[j]);
}

//Secondo esempio di mutazione: a partire da una città estratta pseudocasualmente, si spostano di n posizioni gli indici di m città
void Route::Shift(Random& rnd){

    int n = floor(rnd.Rannyu(1, m_ncity)); //Numero di posizioni di cui shiftare la città
    int m = floor(rnd.Rannyu(1, m_ncity)); //Numero di città da shiftare
    int start = floor(rnd.Rannyu(1, m_ncity)); //Città da cui partire con la mutazione

    //Ciclo esterno sul numero di posizioni di cui shiftare la città
    for(int i = 0; i < n; i++){
        //Ciclo interno sul numero di città da shiftare
        for(int j = m - 1; j >= 0; j--){
            //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 5, 2, 3], 
            //n = 2, m = 2, start = 1, start + i + j = 2 => [1, 2, 3, 4, 5] -> [1, 2, 4, 3, 5], j --
            //start + i + j = 1 => [1, 2, 4, 3, 5] -> [1, 4, 2, 3, 5], i++
            //start + i + j = 3 => [1, 4, 2, 3, 5] -> [1, 4, 2, 5, 3], j --
            //start + i + j = 2 => [1, 4, 2, 3, 5] -> [1, 4, 5, 2, 3]

            //Città in posizione start + i + j scambiata con la città successiva, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_ncity]
            swap(m_route[pbc(start + i + j)], m_route[pbc(start + i + j + 1)]);
        }
    }
}

//Terzo esempio di mutazione: si scambiano le posizioni di m città contigue con quelle di altre m città contigue, controllando che m < N/2    
void Route::Permutation(Random& rnd){   

    int start =  floor(rnd.Rannyu(1, m_ncity)); //Città da cui partire con la mutazione
    int m = floor(rnd.Rannyu(2, m_ncity/2.)); //Numero di città da permutare

    //Ciclo sul numero di città da permutare
    for(int i = 0; i < m; i++){
        //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 5, 2, 3], m = 2, start = 1, start + i = 1, start + i + m = 3, i++, start + i = 2, start + i + m = 4
        //Città in posizione start + i scambiata con la città in posizione start + i + m, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_ncity]
        swap(m_route[pbc(start + i)], m_route[pbc(start + i + m)]);
    }
}

//Quarto esempio di mutazione: si inverte l'ordine in cui le città appaiono nel percorso, per m città con m <= N
void Route::Inversion(Random& rnd){

    int m = floor(rnd.Rannyu(2, m_ncity)); //Numero di città da invertire
    int start = floor(rnd.Rannyu(1, m_ncity)); //Città da cui partire con la mutazione

    //Ciclo esterno sul numero di città da invertire, dimezzato, poiché scambiano le posizioni reciproche delle città
    for (int i = 0; i < m/2; i++) {
        //Esempio: [1, 2, 3, 4, 5] -> [1, 4, 3, 2, 5], m = 2, start = 1, start + i = 1, start + m - i = 3
        //Città in posizione start + i scambiata con la città in posizione start + m - i, controllando di non star modificando la città di indice 0 e di non finire fuori dal range [1, m_ncity]
        swap(m_route[pbc(start + i)], m_route[pbc(start + m - i)]);
    }
}

void Route::Print( int shape ){
    string Shape;
    if (shape == 0){
        Shape = "Circle";
    }
    else{
        Shape = "Square";
    }

    ofstream Route;
    Route.open("Route_"+Shape, ios::app);
    for(int i=0; i < m_ncity; i++ ){
        Route << m_route[i] << "  ";
    }
    Route << endl;
    Route.close();
}

int Route::pbc(int icity){
    //Per non modificare la città di partenza ci spostiamo a quella immediatamente successiva
    if(icity == 0) {
        return 1;
    }

    //Se l'indice della città in cui ci troviamo è superiore al numero di città disponibili, lo diminuiamo fino a rientrare nel giusto range [1, m_ncity]
    while(icity >= m_ncity){
        icity -= (m_ncity - 1);
    }

    return icity;
}

bool Route::operator<(const Route& other) const{
    return GetLength() < other.GetLength();
}