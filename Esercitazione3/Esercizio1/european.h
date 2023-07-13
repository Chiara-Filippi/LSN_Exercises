#ifndef __European_H__
#define __European_H__

#include <cmath>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

#include "random.h"

using namespace std;

class European{

public:

	European();
	~European();
	double AssetContinuo(double, double, double, double, double);
	double AssetDiscreto(double, double, double, double, double, double);
};

#endif