#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include "random.h"
#include "blockav.h"

using namespace std;

void RandomInitializer(Random& rnd){
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
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
   
double error(double sum, double sum2, int iblk) {
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk); //iblk diverso da 0, non mettiamo il controllo
}

Function :: Function(){};
Function :: ~Function(){};

double Function :: EvalUnif(double x) {
    return ( M_PI/2. )*cos( ( M_PI/2.) * x ); 
}

double Function :: EvalImp(double x) {
    return ( M_PI/2. )*cos( ( M_PI/2.) * x ) / ( 2.*( 1. - x ) ); //per la stima dell'integrale
}


BlockAverage :: BlockAverage() : blk_av(n_comp), glob_av(n_comp), glob_av2(n_comp), walker(n_comp) {};
BlockAverage :: ~BlockAverage(){};

void BlockAverage :: Reset(int iblock) {
    blk_norm = 0.;
    stima = 0.;
    errore = 0.;

    if(iblock == 1) {
        fill(blk_av.begin(), blk_av.end(), 0.);
        fill(glob_av.begin(), glob_av.end(), 0.);
        fill(glob_av2.begin(), glob_av2.end(), 0.);
    }

    else {
        fill(blk_av.begin(), blk_av.end(), 0.);
    }
}

void BlockAverage :: Measure(double x, double y, Function& f) {
    walker[iav_unif] = f.EvalUnif(x);
    walker[iav_imp] = f.EvalImp(y);
}

void BlockAverage :: Accumulate(void) {
    for(int i=0; i<n_comp; i++) {
        blk_av[i] = blk_av[i] + walker[i];
    }
    blk_norm += 1.0;
}

void BlockAverage :: Average(int iblock, int L, ofstream &Average1, ofstream &Average2){
    stima = blk_av[iav_unif]/blk_norm;
    glob_av[iav_unif] += stima;
    glob_av2[iav_unif] += stima*stima;
    errore = error(glob_av[iav_unif], glob_av2[iav_unif], iblock);

    Average1 << iblock*L << "\t" << glob_av[iav_unif]/(double)iblock << "\t" << errore << endl;

    stima = blk_av[iav_imp]/blk_norm;
    glob_av[iav_imp] += stima;
    glob_av2[iav_imp] += stima*stima;
    errore = error(glob_av[iav_imp], glob_av2[iav_imp], iblock);

    Average2 << iblock*L << "\t" << glob_av[iav_imp]/(double)iblock << "\t" << errore << endl;

}
