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

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;
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

   /* for(int i=0; i<20; i++){
      cout << rnd.Rannyu() << endl;
   }
	*/

	vector<double> r;
	vector<double> av;
	vector<double> av2;
	vector<double> error_prog;
	vector<double> sum_prog;
	vector<double> su2_prog;
	ofstream output;
	output.open("output1.1");
	
	double M = 1E5;
	double N = 100.;
	int L = int(M/N);
	double sum = 0.;
	double sum2 = 0.;
	
	//Generati M numeri casuali compresi tra 0 e 1
	for( int i=0; i<M; i++) {
		r.push_back(rnd.Rannyu());
		//cout << r[i] << endl;
	}

	//Calcolata la media dei numeri casuali nel blocco N-esimo
	for(int i=0; i<N; i++) {
		for(int j=0; j<L; j++) {
			sum += r[ i*L + j];
		}
		av.push_back(sum/L);
		av2.push_back(av[i]*av[i]);
		//cout << av[i] << endl;
		sum = 0.;
		
		for( int j = 0; j<i+1; j++) {
			sum += av[j]; 
			sum2 += av2[j];
		}
		sum_prog.push_back(sum/(i+1));
		su2_prog.push_back(sum2/(i+1));
		sum = 0.;
		sum2 = 0.;
		if(i == 0){
			error_prog.push_back( sqrt( su2_prog[i] - pow( sum_prog[i], 2 ) ) );
		}
		else{
			error_prog.push_back( sqrt( ( su2_prog[i] - pow( sum_prog[i], 2 ) ) /i ) );
		}
		cout << i*L << "\t" << sum_prog[i] - 0.5 << "\t" << "\t" << error_prog[i] << endl;
		output << i*L << "\t" << sum_prog[i] - 0.5 << "\t" << error_prog[i] << endl;
		
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
