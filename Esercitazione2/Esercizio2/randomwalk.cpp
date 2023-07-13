#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include "random.h"
#include "randomwalk.h"

using namespace std;

RandomWalk :: RandomWalk(){}
// Default constructor, does not perform any action

RandomWalk:: ~RandomWalk(){}
// Default destructor, does not perform any action

//Random Walk discreto: prendiamo un numero pseudocasuale tra [0,1], lo moltiplichiamo per 6 e ne prendiamo la parte intera; in base al valore di quest'ultima facciamo un passo lungo x, y o z, in avanti o indietro

vector<double> RandomWalk:: WalkDiscreta(vector<double> &v, double y){
	y = 6.*y;
	for(double i=0.; i<6.; i++) {
		if(floor(y) == i ) {
			v[i/2.] += pow(-1., i);
		}
	}
	return v;
};

//Random Walk continue: prendiamo due numeri pseudocasuali tra [0,pi] e [0, 2pi], in base all'intervallo in cui ricade facciamo un passo lungo x, y o z, in avanti o indietro

vector<double> RandomWalk:: WalkContinua(vector<double> &v, double theta, double phi){
	v[0] += sin(theta)*cos(phi);
	v[1] += sin(theta)*sin(phi);
	v[2] += cos(theta);
	return v;
};