#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "random.h"
#include "european.h"
#include "blockav.h"

using namespace std;
 
int main (int argc, char *argv[]){

    Random rnd;
    RandomInitializer(rnd);

	double M = 1E4;
	double N = 100.;
	int L = int(M/N);

	//parametri del problema
	double T = 1.;
	double S = 100.;
	double K = 100.;
	double r = 0.1;
	double sigma = 0.25;

	double S_t = 0.;

	European EU;
    BlockAverage BlkAv;
    Function Fun;

	ofstream output_call;
	ofstream output_put;
	output_call.open("diretto_call");
	output_put.open("diretto_put");

    //Ciclo esterno sul numero di blocchi
	for(int i=1; i<=L; i++) {
        BlkAv.Reset(i); //Tutti i vettori e i parametri vengono settati a zero
        //Ciclo interno sul numero di dati generati in ogni blocco
        for(int j=1; j<=N; j++) {
            S_t = EU.AssetContinuo( S, r, sigma, T, rnd.Gauss(0., T) );
            BlkAv.Measure(S_t, K, r, T, Fun); //La S_t viene usata per valutare la call e la put
            BlkAv.Accumulate();
        } 
        BlkAv.Average(i, L, output_call, output_put);
    }

	output_call.close();
	output_put.close();

	output_call.open("discreto_call");
	output_put.open("discreto_put");
	
	double t_iniziale = 0.;
	double t_finale = 1./100.;
	S_t = 100.;

	//Ciclo esterno sul numero di blocchi
	for(int i=1; i<=L; i++) {
        BlkAv.Reset(i); //Tutti i vettori e i parametri vengono settati a zero
        //Ciclo interno sul numero di dati generati in ogni blocco
        for(int j=1; j<=N; j++) {
            while( t_finale <= 1. ) {
                S_t = EU.AssetDiscreto( S_t, r, sigma, t_iniziale, t_finale, rnd.Gauss(0., 1.) );
                t_iniziale += 1./100.;
                t_finale += 1./100.;
            }
            BlkAv.Measure(S_t, K, r, T, Fun); //La S_t viene usata per valutare la call e la put
            BlkAv.Accumulate();
            t_iniziale = 0.;
            t_finale = 1./100.;
            S_t = 100.;
        } 
        BlkAv.Average(i, L, output_call, output_put);
    }

	output_call.close();
	output_put.close();
	
    return 0;
}