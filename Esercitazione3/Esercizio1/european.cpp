#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include "random.h"
#include "european.h"

using namespace std;

European :: European(){}
// Default constructor, does not perform any action

European:: ~European(){}
// Default destructor, does not perform any action

double European:: AssetContinuo(double S, double mi, double sigma, double t, double W) {
	return S*exp( ( mi - ( 1./2. )*( pow( sigma, 2 ) )*t ) + sigma*W );
}

double European:: AssetDiscreto(double S, double mi, double sigma, double t_iniziale, double t_finale, double Z) {
	return S*exp( ( mi - ( 1./2. )*( pow( sigma, 2 ) ) )*( t_finale - t_iniziale ) + sigma*Z*sqrt( t_finale - t_iniziale ) );
}