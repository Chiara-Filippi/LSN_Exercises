#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
//#include "random.h"
#include "blockav.h"

using namespace std;
 
int main (int argc, char *argv[]){

    Random rnd;
    RandomInitializer(rnd);

	ofstream Average1;
	ofstream Average2;
	
	Average1.open("Average1");
	Average2.open("Average2");

	BlockAverage BlkAv;
    Function Fun;
	
	double M = 10000;
	double N = 100.;
    int L = int(M/N);
    double x = 0.;
    
	//Ciclo esterno sul numero di blocchi
	for(int i=1; i<=L; i++) {
        BlkAv.Reset(i); //Tutti i vettori e i parametri vengono settati a zero
        //Ciclo interno sul numero di dati generati in ogni blocco
        for(int j=1; j<=N; j++) {
            x = rnd.Rannyu();
            BlkAv.Measure(x, rnd.NonUnif(x), Fun); //Generato pseudocasualmente un numero tra 0 e 1, conservato nel primo elemento del walker; lo stesso numero viene usato per calcolare la sigma
            BlkAv.Accumulate(); //Si somma per N volte il numero pseudocasuale generato
        } 
        BlkAv.Average(i, L, Average1, Average2);
    }
    
	Average1.close();
	Average2.close();
	
	return 0;
	}