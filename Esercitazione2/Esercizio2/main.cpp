#include "randomwalk.h"
#include "random.h"
#include "blockav.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

    Random *rnd = new Random();
    RandomInitializer(*rnd);

    //Initialize output file
    ofstream output_discreto("output_randomwalkdiscreto");
    ofstream output_continuo("output_randomwalkcontinuo");

    //Initialize random walk object
    RandomWalk RW;

    //Initialize vectors to store positions
    vector<double> v_discreto = {0., 0., 0.};
    vector<double> v_continuo = {0., 0., 0.};

    //Initialize double variables to store the distance
    double norm_discreta = 0.;
    double norm_continua = 0.;

    vector<double> discreto_average_sqrt(100, 0.0);
    vector<double> discreto_block(100, 0.0);
    vector<double> discreto_block_sqrt(100, 0.0);
    vector<double> discreto_average(100, 0.0);
    vector<double> discreto_average2(100, 0.0);

    vector<double> continuo_average_sqrt(100, 0.0);
    vector<double> continuo_block(100, 0.0);
    vector<double> continuo_block_sqrt(100, 0.0);
    vector<double> continuo_average(100, 0.0);
    vector<double> continuo_average2(100, 0.0);

    double discreto_error = 0.0;
    double continuo_error = 0.0;

    int M = 10000;
    int N = 100;
    int L = M/N;

    //Loop over blocks
    for (int j = 1; j <= L; j++) {

        //Loop over steps in block
        for (int k = 1; k <= N; k++) {

            v_discreto = {0., 0., 0.};
            v_continuo = {0., 0., 0.};
                
            //Loop over random walk steps
            for (int i = 0; i < 100; i++) {
                    
                RW.WalkDiscreta(v_discreto, rnd->Rannyu());
                    
                RW.WalkContinua(v_continuo, acos( 2.*rnd->Rannyu() - 1.), rnd->Rannyu(0., 2 * M_PI));
                //angoli θ e φ estratti uniformemente sulla sfera. Non possiamo semplicemente estrarre θ tra 0 e π e φ tra 0 e 2π, poiché l'angolo solido sotteso a un'area infinitesima di sfera è dΩ = sinθ dθ dφ, dunque estrarre uniformemente negli intervalli porta a un accumulo di punti sui poli. Per avere distribuzione uniforme sulla sfera scegliamo u e v uniformemente distribuite su (0,1). φ può essere semplicemente posto = 2π u, dato che dΩ è legato linearmente a dφ. La distribuzione di θ è invece data da: P_θ dθ = P_v | dv/dθ | dθ, P_v = 1 (distribuzione uniforme), P_θ = 1/2 sinθ (distribuzione legata alla forma di dΩ, con normalizzazione ? )
                
                norm_discreta = (pow(v_discreto[0], 2) + pow(v_discreto[1], 2) + pow(v_discreto[2], 2));
                norm_continua = (pow(v_continuo[0], 2) + pow(v_continuo[1], 2) + pow(v_continuo[2], 2));

                //Accumulate the distance to the block vector
                discreto_block[i] += norm_discreta; 
                discreto_block_sqrt[i] += sqrt(norm_discreta);
                    
                continuo_block[i] += norm_continua;
                continuo_block_sqrt[i] += sqrt(norm_continua);

            }
        }
            
        //Compute the block average for each step
        for( int i=0; i<100; i++) {
            discreto_average[i] += discreto_block[i] / (double)L;
                        
            discreto_average_sqrt[i] += (discreto_block_sqrt[i]) / (double)L;
                        
            discreto_average2[i] += pow( discreto_block_sqrt[i] / (double)L, 2 ); // Compute the block square average for each step
                        
            discreto_block[i] = 0.;
            discreto_block_sqrt[i] = 0.;

            continuo_average[i] += continuo_block[i] / (double)L;

            continuo_average_sqrt[i] += (continuo_block_sqrt[i]) / (double)L;
                        
            continuo_average2[i] += pow( continuo_block_sqrt[i] / (double)L, 2 );
                
            continuo_block[i] = 0.;
            continuo_block_sqrt[i] = 0.;
        }
    }

    for (int i = 0; i < 100; i++) {
        discreto_error = sqrt( (discreto_average2[i] / (double)L - pow( (discreto_average_sqrt[i]) / (double)L, 2 ) ) / (double)(L - 1) );

        continuo_error = sqrt( (continuo_average2[i] / (double)L - pow( (continuo_average_sqrt[i]) / (double)L, 2 ) ) / (double)(L - 1) );
        
        output_discreto << i << "\t" << sqrt(discreto_average[i] / double(L)) << "\t" << discreto_error << endl;
        output_continuo << i << "\t" << sqrt(continuo_average[i] / double(L)) << "\t" << continuo_error << endl;

    }
    //Close output file
    output_discreto.close();
    output_continuo.close();

    return 0;
}