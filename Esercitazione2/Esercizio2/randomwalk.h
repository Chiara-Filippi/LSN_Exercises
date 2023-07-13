#ifndef __RANDOMWALK_H__
#define __RANDOMWALK_H__

#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

#include "random.h"

using namespace std;

class RandomWalk{

public:

	RandomWalk();
	~RandomWalk();

	vector<double> WalkDiscreta(vector<double> &, double);
	vector<double> WalkContinua(vector<double> &, double, double);
};

#endif