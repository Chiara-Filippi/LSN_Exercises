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
#include <ostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include "random.h"
#include "Map.h"
#include "Route.h"
#include "Population.h"

int main(){
    //Random 
    Random rnd;
    RandomInitializer(rnd);

    //Input 
    ifstream Input;
    Input.open("input.in");
    int n_population, n_city, shape, nstep;
    string Shape;
    Input >> n_population;
    Input >> n_city;
    Input >> shape;
    Input >> nstep;
    Input.close();
    
    if (shape == 0){
        Shape = "Circle";
    }
    else{
        Shape = "Square";
    }
    
    ofstream Length;
    Length.open("LengthMean_"+Shape);
    
    //Simulation
    Population pop(n_population, n_city, shape);
    for(int i  = 0; i < nstep; i++){
        //Creata una nuova generazione
        pop.NextGen();
        //Mutata la nuova generazione
        pop.Mutation();
        //Ordinata la nuova generazione
        pop.Sorting();
        Length << pop.MeanLength() << endl;
        cout << "qui" << endl;
    }
    Length.close();
    pop.PrintMap(shape);
    pop.PrintBestRoute(shape);

    return 0;
}
