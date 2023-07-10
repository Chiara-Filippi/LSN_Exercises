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

	ofstream output;
	output.open("ChiSq.dat");
	
	double M = 100;
	double N = 10000;
	double n_i = 0.;
	double sum=0.;
	vector<double> r(N);
	
	//Dividiamo [0,1] in M blocchi e contiamo quanti dei numeri pseudocasuali generati stanno nel blocco. Per farlo creaiamo un for esterno che scorra sui blocchi e un for interno che scorra sui numeri pseudocasuali, con un if checkiamo se rientrano nell'intervallo giusto ed eventualmente aumentiamo un contatore
	for(double k=0.; k<100.; k++){
		for(int j=0; j<N; j++) {
			r[j] = rnd.Rannyu();
		}
		for(double i=0.; i<M; i++) {
			for(double j=0.; j<N; j++) {
				if( ( r[j] <= ( 1. + i )/M ) && ( r[j] > i/M ) ){
					n_i ++;
				}
			}
			sum += ( pow( ( n_i - 100.), 2) / ( 100. ) );
			n_i = 0.;
		}
		output << sum << endl;
		sum = 0.;
	}
	
	
    rnd.SaveSeed();
	output.close();
	

	
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
