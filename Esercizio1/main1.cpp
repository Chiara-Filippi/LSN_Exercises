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
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include "random.h"
#include "blockav.h"

using namespace std;
 
int main (int argc, char *argv[]){

    Random rnd;
    RandomInitializer(rnd);

	ofstream Average;
    ofstream Error;
    Average.open("Average.dat");
    Error.open("Error.dat");

    BlockAverage BlkAv;
    Function Fun;
	
	double M = 10000;
	double N = 100.;
    int L = int(M/N);
    
	//Ciclo esterno sul numero di blocchi
	for(int i=1; i<=L; i++) {
        BlkAv.Reset(i); //Tutti i vettori e i parametri vengono settati a zero
        //Ciclo interno sul numero di dati generati in ogni blocco
        for(int j=1; j<=N; j++) {
            BlkAv.Measure(rnd.Rannyu(), Fun); //Generato pseudocasualmente un numero tra 0 e 1, conservato nel primo elemento del walker; lo stesso numero viene usato per calcolare la sigma
            BlkAv.Accumulate(); //Si somma per N volte il numero pseudocasuale generato
        } 
        BlkAv.Average(i, L, Average, Error);
    }
    Average.close();
    Error.close();
	
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
