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

	double sum_std = 0.;
	double sum_exp = 0.;
	double sum_CL = 0.;
	ofstream output;

	output.open("output1");
	
	for(int i=0; i<10000; i++) {
		
		sum_std = rnd.Rannyu(0., 1.); //dado standard
		sum_exp = rnd.Exp(1.); //dado esponenziale
		sum_CL = rnd.CauchyLorentz(0., 1.); //dado Cauchy-Lorentz
		
		output << sum_std << "\t" << sum_exp << "\t" << sum_CL << endl;
		sum_std = 0.;
		sum_exp = 0.;
		sum_CL = 0.;
		
	}

	output.close();
	
	output.open("output2");
	
	for( int i=0; i<10000; i++ ) {
		
		for( int j=0; j<2; j++ ) {
			
			sum_std += rnd.Rannyu(0., 1.);
			sum_exp += rnd.Exp(1.);
			sum_CL += rnd.CauchyLorentz(0., 1.);
			
		}
		
		output << sum_std/2. << "\t" << sum_exp/2. << "\t" << sum_CL/2. << endl;
		sum_std = 0.;
		sum_exp = 0.;
		sum_CL = 0.;
		
	}

	output.close();
	
	output.open("output10");
	
	for( int i=0; i<10000; i++ ) {
		
		for( int j=0; j<10; j++ ) {
			
			sum_std += rnd.Rannyu(0., 1.);
			sum_exp += rnd.Exp(1.);
			sum_CL += rnd.CauchyLorentz(0., 1.);
			
		}
		
		output << sum_std/10. << "\t" << sum_exp/10. << "\t" << sum_CL/10. << endl;
		sum_std = 0.;
		sum_exp = 0.;
		sum_CL = 0.;
		
	}

	output.close();

	output.open("output100");
	
	for( int i=0; i<10000; i++ ) {
		
		for( int j=0; j<100; j++ ) {
		
			sum_std += rnd.Rannyu(0., 1.);
			sum_exp += rnd.Exp(1.);
			sum_CL += rnd.CauchyLorentz(0., 1.);
			
		}
		
		output << sum_std/100. << "\t" << sum_exp/100. << "\t" << sum_CL/100. << endl;
		sum_std = 0.;
		sum_exp = 0.;
		sum_CL = 0.;
		
	}

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
