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

double Function :: EvalCall(double x, double K) {
    return max(x - K, 0.); //stima della call
}
double Function :: EvalPut(double x, double K) {
    return max( K - x, 0.); //stima della put
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

void BlockAverage :: Measure(double x, double K, double r, double T, Function& f) {
    walker[icall] = f.EvalCall(x, K) * exp( -r*T );
    walker[iput] = f.EvalPut(x, K) * exp( -r*T );
}

void BlockAverage :: Accumulate(void) {
    for(int i=0; i<n_comp; i++) {
        blk_av[i] = blk_av[i] + walker[i];
    }
    blk_norm += 1.0;
}

void BlockAverage :: Average(int iblock, int L, ofstream &Call, ofstream &Put){
    stima = blk_av[icall]/blk_norm;
    glob_av[icall] += stima;
    glob_av2[icall] += stima*stima;
    errore = error(glob_av[icall], glob_av2[icall], iblock);

    Call << iblock*L << "\t" << glob_av[icall]/(double)iblock << "\t" << errore << endl;

    stima = blk_av[iput]/blk_norm;
    glob_av[iput] += stima;
    glob_av2[iput] += stima*stima;
    errore = error(glob_av[iput], glob_av2[iput], iblock);

    Put << iblock*L << "\t" << glob_av[iput]/(double)iblock << "\t" << errore << endl;

}
