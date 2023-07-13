#include "Map.h"
#include "Route.h"
#include "Population.h"
#include "mpi.h"

using namespace std;

int pbc(vector<int> order, int i) {
    return i - order.size()*floor( (double)i/order.size() );
}

void Shuffle(Random& rnd, vector<int>& order) {
    int i = floor( rnd.Rannyu(0, order.size() ) );
    int j = floor( rnd.Rannyu(0, order.size() ) );
    swap(order[i], order[j]);
}

int main(int argc, char* argv[]){

    MPI_Init(&argc, &argv);

    //Mappa 
    ifstream Capitals("American_capitals.in");

    Map map(50, Capitals);

    Capitals.close();

    map.PrintPosition();         

    //rank = quale processore sta lavorando, size = numero di processore
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //Inizializzazione generatore di numeri casuali. Uno condiviso, gli altri per singolo processo.
    Random shared_rnd;
    RandomInitializer(shared_rnd,0);

    Random rnd;
    RandomInitializer(rnd,rank);

    //Parametri simulazione
    int nindividuals = 500;
    int ngenerations = 2000;
    int migrate = 0; //0 se non voglio migrare, 1 se voglio farla
    int N_migr = 10;

    vector<int> sending_order; //Vettore di indici per lo scambio
    for(int i=0; i < size; i++){
        sending_order.push_back(i);
    }
    for(int i=0; i < 10; i++){
        Shuffle(shared_rnd,sending_order);
    }
 
    int current_rank, send_to, received_from;
    Route best(50, shared_rnd, map);
    vector<int> best_path;

    Population pop(rnd, nindividuals, 50, map);

    
    if(migrate == 0){ //Senza migrazione

        ofstream AverageLengths("AverageLengths_nomig_" + to_string(rank));

        for(int i = 0; i < ngenerations; i++){

            //Creata una nuova generazione
            pop.NextGen();
            //Mutata la nuova generazione
            pop.Mutation();
            //Ordinata la nuova generazione
            pop.Sorting();

            AverageLengths << setw(10) << i << setw(10) << pop.MeanLength() << endl;
                
        }
        AverageLengths.close();

    }
    else{ //Con migrazione

        ofstream AverageLengths("AverageLengths_mig_" + to_string(rank));

        for(int i = 0; i < ngenerations; i++){
            //Creata una nuova generazione
            pop.NextGen();
            //Mutata la nuova generazione
            pop.Mutation();
            //Ordinata la nuova generazione
            pop.Sorting();

            AverageLengths << setw(10) << i << setw(10) << pop.MeanLength() << endl;

            if( i!=0 and i%N_migr==0 ){

                for(int j = 0; j < size; j++){
                    //Continente corrente
                    current_rank = sending_order[pbc(sending_order, j)];
                    //Si manda al continente successivo  
                    send_to = sending_order[pbc(sending_order, j+1)];  
                    //Si riceve dal continente precedente
                    received_from = sending_order[pbc(sending_order, j-1)];
                
                    if(rank == current_rank){
                        
                        best = pop.GetBestPopulation();
                        best_path = best.GetRoute();
                        MPI_Request request1;
                        MPI_Request request2;

                        MPI_Isend(best_path.data(), best.GetNcity(), MPI_INT, send_to, 0, MPI_COMM_WORLD, &request1);
                        MPI_Wait(& request1, MPI_STATUS_IGNORE);

                        MPI_Irecv(best_path.data(), best.GetNcity(), MPI_INT, received_from, 0, MPI_COMM_WORLD, & request2);
                        MPI_Wait(& request2, MPI_STATUS_IGNORE);
                  
                        best.SetRoute(best_path);
                        pop.SetIndividual(best, pop.GetPopulation() - 1);        //sostituisci individuo peggiore con nuovo individuo
        
                    }

                }
                for(int i=0; i<10; i++) {
                    Shuffle(shared_rnd, sending_order);
                }
                pop.Sorting();
            }
            
        }

        AverageLengths.close();
    
    }

    best = pop.GetBestPopulation();

    //Buffer, i.e. vettori di input e di output
    vector<int> send = best.GetRoute();
    vector<int> receive(size * best.GetNcity());

    //Vettore receive riempito con i migliori individui trovati da ciascun nodo
    MPI_Gather(send.data(), best.GetNcity(), MPI_INT, receive.data(), best.GetNcity(), MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {

        //Immagazzinata la miglior popolazione di ciascun nodo
        Population population_best(shared_rnd, size, 50,  map);

        for (int i = 0; i < size; i++) {
            //Riempito population_best a partire dal vettore di receive
            best.SetRoute(vector<int>(receive.begin() + (i * best.GetNcity()), receive.begin() + ((i + 1) * best.GetNcity())));
            population_best.SetIndividual(best, i);
        }

        population_best.Sorting();
        
        population_best.PrintBestRoute();

        ofstream Length("Length_nomig");
        Length << population_best.RouteLength(0) << endl;

        Length.close();

    }

    MPI_Finalize();

    return 0;

}
