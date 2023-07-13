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
#include "Map.h"
#include "Route.h"
#include "Population.h"

using namespace std;

void RandomInitializer(Random& rnd){
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } 
    else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;
}

double Error(double sum, double sum2, int iblk) {
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}

//Costruttore
Population::Population(int npopulation, int ncities, int shape){

    RandomInitializer(rnd);

    //Generata la mappa
    m_map = Map(shape, ncities, rnd);

    //Generati i percorsi
    m_npopulation = npopulation;
    m_pop.resize(m_npopulation);

    for(int i = 0; i < m_npopulation; i++){
        m_pop[i] = Route(ncities, rnd, m_map);; 
    }
}

Population::~Population () {}

//Si creano due nuovi elementi della popolazione tramite il crossover
void Population::NextGen(){
    int father_index, mother_index;
    Sorting();
    for(int i = 0; i < m_npopulation/2; i++){        //n/2 couples
        father_index = Selection();
        mother_index = Selection();
        Crossover(father_index, mother_index);
    }
    Sorting();  
    m_pop.resize( m_npopulation ); //eliminati gli individui nella seconda metà della popolazione
}

//Selezionato un individuo dalla popolazione. La selezione va effettuata sulla popolazione ordinata, in modo da preferire gli individui nelle prime posizioni del vettore
int Population::Selection(){
    double p = 5.0;
    return (int)( m_npopulation * pow(rnd.Rannyu(), p) );
}

//Effettuate le mutazioni, ciascuna con probabilità diversa, < 10%
void Population::Mutation(){
    double mutation;
    for(int i=0; i < m_npopulation; i++ ){
        mutation = rnd.Rannyu();
        if(mutation < 0.04){
            m_pop[i].Shift(rnd);
        } 
        if( 0.04 <= mutation < 0.08){
            m_pop[i].Inversion(rnd);
        }
        if( 0.08 <= mutation < 0.12 ){
            m_pop[i].Permutation(rnd);
        } 
        if( 0.12 <= mutation < 0.16){
            m_pop[i].PairPermutation(rnd);
        }
    }
}

//Effettuato il crossover
void Population::Crossover (int father, int mother) {
    //Crea dei percorsi figli come copia dei genitori
    Route child1 = m_pop[father];
    Route child2 = m_pop[mother];
    //Effettuato il crossover con probabilità del 90%
    if( rnd.Rannyu() < 0.90){
        int support1 = 0;
        int support2 = 0;
        int cut = floor(rnd.Rannyu(1, m_map.GetNcity() - 2)); //Selezionato randomicamente un punto di taglio

        //Create dei nuovi percorsi figli, identici ai precedenti fino al punto di taglio
        vector<int> child1_route;
        vector<int> child2_route;
        child1_route = child1.GetRoute();
        child1_route.resize(cut);
        child2_route = child2.GetRoute();
        child2_route.resize(cut);

        //Ciclo sulle città che compongono i percorsi genitori
        for(int i = 0; i < m_map.GetNcity(); i ++){
            support1 = 0;
            support2 = 0;
            //Controlliamo se le città che compaiono nel percorso child1 compaiono anche nella madre
            for(int j = 0; j < child1_route.size() ; j++){
                if( m_pop[mother].GetCityIndex(i) == child1_route[j] ){
                    support1 ++;
                    break;
                }
            }
            //Se la città i-esima della madre non compare già nel percorso figlio, la aggiungiamo
            if (support1 == 0 ) child1_route.push_back(m_pop[mother].GetCityIndex(i));

            //Medesimo ragionamento per child2 e per il padre
            for(int j = 0; j < child2_route.size(); j++){
                if( m_pop[father].GetCityIndex(i) == child2_route[j] ){
                    support2 ++;
                    break;
                }
            }
            if (support2 == 0 ) child2_route.push_back(m_pop[father].GetCityIndex(i));
        }
        //Aggiorniamo il percorso dei figli
        child1.SetRoute(child1_route);
        child2.SetRoute(child2_route);
    }
    //Aggiungiamo i nuovi individui alla popolazione
    m_pop.push_back(child1);
    m_pop.push_back(child2);
}

//Ordinati i percorsi in lunghezza crescente
void Population::Sorting(){
    for(int i = 0; i < m_pop.size(); i++){
        m_pop[i].CalculateLength(m_map);
    }
    sort(m_pop.begin(), m_pop.end());       //with overridden < 
}

//Stampato il percorso migliore, ossia quello più corto; richiede un sorting preliminare
void Population::PrintBestRoute(int shape){
    m_pop[0].Print(shape);
}

void Population::PrintMap(int shape){
    m_map.PrintPosition(shape);
}

//Stampata la lunghezza media sulla migliore metà della popolazione; richiede un sorting preliminare
double Population::MeanLength(){   //of half population
    double sum = 0.0;
    for(int i = 0; i < m_npopulation/2; i++){
        sum += m_pop[i].GetLength();
    }
    return sum/(m_npopulation/2);
}

int Population::GetPopulation(){
    return m_npopulation;
}
