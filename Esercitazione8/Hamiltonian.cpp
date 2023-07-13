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
#include <unistd.h>
#include "random.h"
#include "Hamiltonian.h"


using namespace std;

    void RandomInitializer(Random& rnd){
        int seed[4];
        int p1, p2;
        ifstream Primes("Primes");
        if (Primes.is_open()){
            Primes >> p1 >> p2 ;
        } 
        else cerr << "PROBLEM: Unable to open Primes" << endl;
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
    }

    double Error(double sum, double sum2, int iblk) {
        return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
    }

    //Costruttore
    Hamiltonian :: Hamiltonian(double mi, double sigma, double delta, int nblk, int nstep){

        RandomInitializer(rnd);

        m_mi = mi;
        m_sigma = sigma;
        m_delta = delta;
        m_nblk = nblk;
        m_nstep = nstep;
        
        x = 0.;
    
        return;
    }

    //Distruttore
    Hamiltonian :: ~Hamiltonian() {}

    vector<double> Hamiltonian :: Eigenvalue(int Print){ //l'intero Print parametrizza il dover stampare i risultati ottenuti su file oppure no

        vector<double> H_and_err;    //Vettore che conserva l'autovalore di H e il suo errore

        for(int iblk=1; iblk <= m_nblk; iblk++) {
            Reset(iblk);   //Reset block averages
            for(int istep=1; istep <= m_nstep; istep++){
                Move();
                Measure();
                Accumulate(); //Update block averages
            }
            H_and_err = Averages(iblk, Print); 
        }

        return H_and_err;
    }

    void Hamiltonian :: PsiSquareHisto(int nbin, double xmax, int nstep) {

        vector<double>  PsiSquare(nbin+1);     //Vettore per conservare il numero di elementi in ciascun bin
        double bin_size = 2.0*xmax/nbin;  //Larghezza dei bin ottenuta dividendo l'intervallo [-xmax, xmax] per il numero di bin
        int out = 0;                      //Variabile per contare quante x stanno fuori dall'intervallo [-xmax, xmax], nel caso in cui la generazione di x_new con probabilità uniforme esca dal range
        for(int i = 1; i <= nstep; i++){
            Move();
            if(x > - xmax && x < xmax){ //istogramma riempito se la x cade nel range
                //( x + xmax )/bin_size fornisce un intero tra 0 e il massimo numero di bin
                PsiSquare[static_cast<int>((x + xmax) / bin_size)] += 1;
            } 
            else {
                out ++;
            }
        }
        cout << "Number of points out of [ " << - xmax <<", " << xmax << " ] in function PsiSquareHisto = " << out << endl;

        //Normalizzazione
        double sum = 0.;

        for(int i = 0; i <= nbin; i++){
            sum += PsiSquare[i];
        }

        double mean = sum/(double)nbin;

        for(int i = 0; i <= nbin; i++){
            PsiSquare[i] = PsiSquare[i]/(2.0 * xmax * mean); //Funzione d'onda normalizzata con l'ampiezza del bin e con il suo valor medio su tutti i bin
        }

        //Print 
        ofstream PsiSquareHist;
        PsiSquareHist.open("PsiSquareHistogram");
        for(int i=0; i<=nbin; i++){
            PsiSquareHist << -xmax + bin_size * i << setw(18) << PsiSquare[i] << endl;
        }
        PsiSquareHist.close();
    }

    void Hamiltonian::SetParameters(double mi, double sigma){
        m_mi = mi;
        m_sigma = sigma;
    }

    void Hamiltonian::SetBlockParameters(int nblk, int nstep){
        m_nblk = nblk;
        m_nstep = nstep;
    }

    double Hamiltonian::GetMean(){
        return m_mi;
    }

    double Hamiltonian::GetSigma(){
        return m_sigma;
    }

    double Hamiltonian::GetDelta(){
        return m_delta;
    }

    double Hamiltonian::Unif(){
        return rnd.Rannyu();
    }

    void Hamiltonian::Move() {
        double p;
        double xnew;

        //generata con probabilità uniforme una nuova x, spostata di poco dalla vecchia x. Vogliamo stare nel range [-3, 3], x è inizialmente generata gaussianamente con media 0. Sommando un numero pseudocasuale compreso tra -1 e 1 e scalandolo per l'ampiezza del range, i.e. m_delta = 3, riusciamo a esplorare tutto il range
        xnew = x + GetDelta()*(rnd.Rannyu(-1, 1)); 
        p = PsiSquare(xnew)/PsiSquare(x);

        if(rnd.Rannyu() < p){ //mossa accettata se p risulta maggiore di un numero pseudocasuale generato tra 0 e 1, algoritmo di Metropolis
            x = xnew;
            accepted ++;
        }
        attempted ++;
        //cout << (double)accepted/(double)attempted << endl;
        return;
    }

    double Hamiltonian::PsiSquare(double x) { //Calcolo del modulo quadro della funzione d'onda
        double mi = GetMean();
        double sigma = GetSigma();
        return pow( exp( -1./2. * pow( ( x - mi )/( sigma ), 2 ) ) + exp( -1./2. * pow( ( x + mi )/( sigma ), 2 ) ), 2);
    }

    void Hamiltonian::Measure() { 
        
        //Calcolo dell'energia potenziale
        double Epot = pow(x, 4) - (5./2.) * pow(x, 2);

        double mi = GetMean();
        double sigma = GetSigma();

        //Ekin calcolata a partire dalla forma analitica della derivata seconda della funzione d'onda
        double term1 = exp( -1./2. * pow( ( x - mi )/( sigma ), 2 ) ) * ( pow( ( x - mi )/( sigma ), 2 ) - 1. )/( pow( sigma, 2 ) );
        double term2 = exp( -1./2. * pow( ( x + mi )/( sigma ), 2 ) ) * ( pow( ( x + mi )/( sigma ), 2 ) - 1. )/( pow( sigma, 2 ) );
        double den =  exp( -1./2. * pow( ( x - mi )/( sigma ), 2 ) ) + exp( -1./2. * pow( ( x + mi )/( sigma ), 2 ) );
        double Ekin = -1./2. * pow( hbar, 2 )/m * ( term1 + term2 )/( den );

        //Calcolo dell'energia totale
        Energy = (Epot + Ekin);

        return;
    }

    void Hamiltonian::Reset(int iblk) { 
        if(iblk == 1){
            glob_av = 0;
            glob_av2 = 0;
        }

        blk_av = 0;
    
        blk_norm = 0;
        attempted = 0;
        accepted = 0;
    }


    void Hamiltonian::Accumulate(void){
        blk_av = blk_av + Energy;  
        blk_norm = blk_norm + 1.0;
    }

    vector<double> Hamiltonian::Averages(int iblk, int Print){ 
        estimate_energy = blk_av/blk_norm;
        glob_av += estimate_energy;
        glob_av2 += pow(estimate_energy, 2);
        err_energy = Error(glob_av, glob_av2, iblk);

        if(Print){ //stampata la media a blocchi delll'energia per il set di parametri ottimale
            ofstream Metro;
            Metro.open("Energy_Metro", ios::app);
            Metro << iblk << setw(18) << estimate_energy << setw(18) << glob_av/(double)iblk << setw(18) << err_energy << endl;
            Metro.close();
        }
        
        return {glob_av/(double)iblk, err_energy};
    }