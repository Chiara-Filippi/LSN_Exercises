
#ifndef __BlockAverage__
#define __BlockAverage__

#include "random.h"

using namespace std;

void RandomInitializer (Random&);
   
double error(double, double, int);

class Function {
    public:
        // Default constructor
        Function();
        // Destructor
        ~Function();
        double Eval(double);
    };

class BlockAverage{
    public:
    // Default constructor
    BlockAverage();
    // Destructor
    ~BlockAverage();

    void Reset(int);

    void Measure(double, Function&);

    void Accumulate(void);

    void Average(int, int, ofstream&);

private:
    int n_comp = 1;
    const int iav = 0; //indicizza la componente del walker

    vector<double> walker;

    double blk_norm = 0.;
    vector<double> blk_av, glob_av, glob_av2;
    double stima = 0.;
    double errore = 0.;


};

#endif // __BlockAverage__

