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
#include <unistd.h>
#include "Monte_Carlo_ISING_1D.h"

using namespace std;

int main() { 

  Input(); //Inizialization

  if(term) { //Termalizzazione
    temp = 2.0;
    cout << temp << endl;
    sleep(1);
    beta = 1./temp;
    for(int iblk=1; iblk <= nblk; ++iblk){ //Simulation
      Reset(iblk);   //Reset block averages
      for(int istep=1; istep <= nstep; ++istep){
          Move(metro);
          Measure();
          Accumulate(); //Update block averages
      }
      Averages(iblk);   //Print results for current block
    }
    ConfFinal(); //Write final configuration
  }
  
  //Dalla termalizzazione si nota che bastano circa 200 blocchi per termalizzare, dunque si fa girare a vuoto la simulazione per tale numero di step senza produrre dati
  else{
    double interval = 1.5; //intervallo di temperature in cui spazia la simulazione
    double gap = 0.01;

    for(temp = 2.0; temp >= 0.5; temp -= gap){
        Input(); //Inizialization
        beta = 1./temp;
        
        //Termalizzazione 
        for(int iblk=1; iblk <= 500; iblk++) {
          Move(metro);
        }

        for(int iblk=1; iblk <= nblk; ++iblk){ //Simulation
            Reset(iblk);   //Reset block averages
            for(int istep=1; istep <= nstep; ++istep){
                Move(metro);
                Measure();
                Accumulate(); //Update block averages
            }
        Averages(iblk);   //Print results for current block
        }
        ConfFinal(); //Write final configuration
    }

  }
    
  return 0;
}


void Input(void)
{
  ifstream ReadInput;

  cout << "Classic 1D Ising model             " << endl;
  cout << "Monte Carlo simulation             " << endl << endl;
  cout << "Nearest neighbour interaction      " << endl << endl;
  cout << "Boltzmann weight exp(- beta * H ), beta = 1/T " << endl << endl;
  cout << "The program uses k_B=1 and mu_B=1 units " << endl;

//Read seed for random numbers
   int p1, p2;
   ifstream Primes("Primes");
   Primes >> p1 >> p2 ;
   Primes.close();

   ifstream input("seed.in");
   input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
   rnd.SetRandom(seed,p1,p2);
   input.close();
  
//Read input informations
  ReadInput.open("input.dat");

  ReadInput >> term;
  cout << "Termalization = " << term << endl; //aggiunto un input per selezionare se effettuare la termalizzazione o se simulare i dati in funzione della temperatura
  //beta = 1.0/temp;
  cout << "Temperature = " << temp << endl;

  ReadInput >> nspin;
  cout << "Number of spins = " << nspin << endl;

  ReadInput >> J;
  cout << "Exchange interaction = " << J << endl;

  ReadInput >> h;
  cout << "External field = " << h << endl << endl;
    
  ReadInput >> metro; // if=1 Metropolis else Gibbs

  ReadInput >> nblk;

  ReadInput >> nstep;

  if(metro==1) cout << "The program perform Metropolis moves" << endl;
  else cout << "The program perform Gibbs moves" << endl;
  cout << "Number of blocks = " << nblk << endl;
  cout << "Number of steps in one block = " << nstep << endl << endl;
  ReadInput.close();


//Prepare arrays for measurements
  iu = 0; //Energy
  ic = 1; //Heat capacity
  im = 2; //Magnetization
  ix = 3; //Magnetic susceptibility
 
  n_props = 4; //Number of observables

//initial configuration
  for (int i=0; i<nspin; ++i)
  {
    if(rnd.Rannyu() >= 0.5) s[i] = 1;
    else s[i] = -1;
  }
  
//Evaluate energy etc. of the initial configuration
  Measure();

//Print initial values for the potential energy and virial
  cout << "Initial energy = " << walker[iu]/(double)nspin << endl;
}


void Move(int metro)
{
    int o;
    double p, energy_old, energy_new, sm;
    double energy_up, energy_down;
    double A, r;

    for(int i=0; i<nspin; ++i){
    //Select randomly a particle (for C++ syntax, 0 <= o <= nspin-1)
    o = (int)(rnd.Rannyu()*nspin);
    if(metro==1){ //Metropolis
			
			energy_old = Boltzmann(s[o], o);
			s[o] *= -1; //flip dello spin
			energy_new = Boltzmann(s[o], o);
			A = min(1., exp( -beta*( energy_new - energy_old) ) );
			
			if( A != 1. ){
				r = rnd.Rannyu(); //numero pseudocasuale tra 0 e 1
				
        if( r > A ){
          s[o] *= -1; //riflippiamo lo spin se la mossa non è accettata
        }
        else {
          accepted += 1;
        }
				
			}
			
    }
    else{ //Gibbs sampling
        energy_up = Boltzmann(1, o);
        energy_down = Boltzmann(-1, o);
		    //formula per Gibbs con delta di energia, per passare da spin +1 a spin -1
		    p = 1./( 1. + exp( -beta*( energy_up - energy_down ) ) ); //scelto il +, probabilità di mettere lo spin a -1
        r = rnd.Rannyu();
        
        if( r < p ) { //mossa accettata, spin messo a -1
            s[o] = -1;
        }
        else {
            s[o] = +1;
        }
        accepted += 1;
			
    }
	attempted += 1;	
  }
	
}

double Boltzmann(int sm, int ip) //sm spin up o down (+1 o -1) in posizione ip
{
  double ene = -J * sm * ( s[Pbc(ip-1)] + s[Pbc(ip+1)] ) - h * sm;
  return ene;
}

void Measure()
{
    double u = 0.0, m = 0.0, c = 0.0, chi = 0.0; 

    //cycle over spins
    for (int i=0; i<nspin; ++i)
    {
        u += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]);
        //c += pow( -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]), 2 );
        chi += s[i];
        m += s[i];
    }
    walker[iu] = u;
    walker[ic] = pow(u, 2);
    walker[ix] = beta*( pow( chi, 2 ) )/(double)nspin;
    walker[im] = m/(double)nspin;
}


void Reset(int iblk) //Reset block averages
{
   
   if(iblk == 1)
   {
       for(int i=0; i<n_props; ++i)
       {
           glob_av[i] = 0;
           glob_av2[i] = 0;
       }
   }

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = 0;
   }
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}


void Accumulate(void) //Update block averages
{

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = blk_av[i] + walker[i];
   }
   blk_norm = blk_norm + 1.0;
}


void Averages(int iblk) //Print results for current block
{
    
    ofstream Ene, Heat, Mag, Chi;
    const int wd=18;
    
    cout << "Block number " << iblk << endl;
    cout << "Acceptance rate " << accepted/attempted << endl << endl;

    string sampling;
    if(metro) {
        sampling = "Metro";
    }
    else {
        sampling = "Gibbs";
    }

    if(h == 0.) {
      if(term) {
        Ene.open(sampling + "_term_ene.0", ios::app);
      }
      else{
        Ene.open(sampling + "_ene.0",ios::app);
      }
        stima_u = blk_av[iu]/blk_norm/(double)nspin; //Energy
        glob_av[iu]  += stima_u;
        glob_av2[iu] += stima_u*stima_u;
        err_u=Error(glob_av[iu],glob_av2[iu],iblk);

        Heat.open(sampling + "_heat.0",ios::app);
        stima_c = pow(beta, 2) * ( blk_av[ic]/blk_norm - pow( blk_av[iu]/blk_norm, 2 ) ) / (double)nspin; //Heat capacity
        glob_av[ic]  += stima_c;
        glob_av2[ic] += stima_c*stima_c;
        err_c=Error(glob_av[ic],glob_av2[ic],iblk);

        Chi.open(sampling + "_chi.0",ios::app);
        stima_x = blk_av[ix]/blk_norm; //Magnetic susceptibility
        glob_av[ix]  += stima_x;
        glob_av2[ix] += stima_x*stima_x;
        err_x=Error(glob_av[ix],glob_av2[ix],iblk);
    }
    else{
        Mag.open(sampling + "_mag.0.02",ios::app);
        stima_m = blk_av[im]/blk_norm; //Magnetic susceptibility
        glob_av[im]  += stima_m;
        glob_av2[im] += stima_m*stima_m;
        err_m=Error(glob_av[im],glob_av2[im],iblk);
    }
    
    //Se term == 1, stampiamo l'energia per ogni blocco, in modo da capire a che blocco avvenga la termalizzazione
    if(term) {
      Ene << setw(wd) << temp << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
      Ene.close();
    }

    else{
      //Si stampa solo l'ultimo risultato della simulazione
      if(iblk == nblk) {
          if(h == 0) {
              Ene << setw(wd) << temp << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
              Ene.close();
              Heat << setw(wd) << temp << setw(wd) << iblk <<  setw(wd) << stima_c << setw(wd) << glob_av[ic]/(double)iblk << setw(wd) << err_c << endl;
              Heat.close();
              Chi << setw(wd) << temp << setw(wd) << iblk <<  setw(wd) << stima_x << setw(wd) << glob_av[ix]/(double)iblk << setw(wd) << err_x << endl;
              Chi.close();
          }
          else{
              Mag << setw(wd) << temp << setw(wd) << iblk <<  setw(wd) << stima_m << setw(wd) << glob_av[im]/(double)iblk << setw(wd) << err_m << endl;
              Mag.close();
          }
      }
    }
    cout << "----------------------------" << endl << endl;
}


void ConfFinal(void)
{
  ofstream WriteConf;

  cout << "Print final configuration to file config.final " << endl << endl;
  WriteConf.open("config.final");
  for (int i=0; i<nspin; ++i)
  {
    WriteConf << s[i] << endl;
  }
  WriteConf.close();

  rnd.SaveSeed();
}

int Pbc(int i)  //Algorithm for periodic boundary conditions
{
    if(i >= nspin) i = i - nspin;
    else if(i < 0) i = i + nspin;
    return i;
}

double Error(double sum, double sum2, int iblk)
{
    if(iblk==1) return 0.0;
    else return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)(iblk-1));
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
