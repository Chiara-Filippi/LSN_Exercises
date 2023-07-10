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

	ofstream Buffon;
    Buffon.open("Buffon.dat");

    BlockAverage BlkAv;
    Function Fun;
	
	double M = 10000;
	double N = 100.;
    int L = int(M/N);
    double d = 1.;
    double l = 0.75;
    
    double x = 2.;
    double y = 2.;
    double theta = 0.;
    double y_inf = 0.;
    double y_sup = 0.;
    int N_thr = 0;
    int N_hit = 0;
    double pi = 0.;

	//Ciclo esterno sul numero di blocchi
	for(int i=1; i<=L; i++) {
        BlkAv.Reset(i); //Tutti i vettori e i parametri vengono settati a zero
        //Ciclo interno sul numero di dati generati in ogni blocco
        for(int j=1; j<=N; j++) {
            x = 2.;
            y = 2.;
            //Estratte due variabili in un quadrato di lato 2, verificato che cadano nella circonferenza unitaria, in modo da calcolare l'angolo con cui l'ago cade sulla griglia. Finché non siamo nella circonferenza unitaria continuiamo a ri-estrarre.
            while( sqrt( pow(x, 2) + pow(y, 2) ) > 1. ) {
                x = rnd.Rannyu(-1., 1.); //ascissa estratta pseudocasualmente tra -1 e 1
                y = rnd.Rannyu(-1., 1.); //ordinata estratta pseudocasualmente tra -1 e 1
            }
            //Controllo sul segno della y; se >0, angolo preso tra 0 e pi greco tramite la funzione arccos, se <0 angolo preso tra 0 e -pi greco
            if( y >= 0 ) {
                theta = acos( x / ( sqrt ( pow(x, 2) + pow(y, 2) ) ) );
            }
            
            else {
                theta = - acos( x / ( sqrt ( pow(x, 2) + pow(y, 2) ) ) );
            }

            //Estratta pseudocasualmente tra 0 e d la y dell'estremo inferiore dell'ago
            y_inf = rnd.Rannyu(0, d);
            //Si somma la lunghezza dell'ago moltiplicata per il seno dell'angolo ottenuto alla y dell'estremo inferiore, in modo da ottenere la y dell'estremo superiore, e si verifica se essa cade sopra d o sotto 0, i.e. se l'ago interseca una riga
            y_sup = y_inf + l*sin(theta);

            if( y_sup >= d || y_sup <= 0 ) {
                N_hit++;
            }

            //Se il primo tiro non va a buon fine, si riparte con il ciclo in modo da non dividere per zero
            if( N_hit == 0 ) { 
                j--;
            }

            else {
                //Stimato il pi greco con la formula di Buffon
                pi = ( 2. * l * (double)N_thr )/( (double)N_hit * d );
                BlkAv.Measure(pi, Fun); //Conservata nel walker la stima effettuata
                BlkAv.Accumulate(); //Si somma per N volte il numero pseudocasuale generato
                N_thr ++;
            }
        } 
        BlkAv.Average(i, L, Buffon);
    }
    
    Buffon.close();
	
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
