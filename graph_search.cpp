#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <sstream>
#include "headers/GNN.h"
#include "headers/MRNG.h"

using namespace std;

int main (int argc, char* argv[]) {

    if(argc < 11) {
        cout<<"Please provide arguments in the wanted format!"<<endl;
        exit(1);
    }

    string input_file = argv[2];
    string query_file;

    //Query file is an optional command line argument
    if ( strcmp(argv[4],"–k") ) {
        query_file = argv[4] ;
        argv++;                     //Next character will be "-k" => skip reading it , move into next one 
    }

    //K-Nearest neighbours in Graph
    int k = 50 ;                       
    //If there is a value after "-k", update k with that
    if ( strcmp(argv[5],"-E") ) {
        k = atoi(argv[5]) ;
        argv++;                     
    }
    
    //Number of Expansions
    int E = 30 ;                       //Default Value
    if ( strcmp(argv[6],"-R") ) {
        E = atoi(argv[6]) ;
        argv++;                     
    }

    if(E > k) {
        cout<<"E should be smaller or equal to k!Please rerun with different parameters!"<<endl;
        exit(1);
    }

    //Number of Random Restarts
    int R = 1 ;                       //Default Value
    if ( strcmp(argv[7],"-N") ) {
        R = atoi(argv[7]) ;
        argv++;                     
    }

    //Wanted Nearest Neighbours
    int N = 1 ;                       //Default Value
    if ( strcmp(argv[8],"-l")) {
        N = atoi(argv[8]) ;
        argv++;                     
    }    

    //Number of candidates (only used for MRNG)
    int l = 20 ;                       //Default Value
    if ( strcmp(argv[9],"-m")) {
        l = atoi(argv[9]) ;
        argv++;                     
    }

    if(l < N) {
        cout<<"N should be smaller or equal to l!Please rerun with different parameters!"<<endl;
        exit(1);
    }

    //Method to use (1 for GNNS, 2 for MRNG)
    int m = atoi(argv[10]) ;                       //Default Value

    //Output file is optional command line argument
    string output_file ;
    if (argv[12] != NULL) {
        output_file = argv[12] ;
    }  

    //Check for extra datafiles which if exist refer to initial dataspaces               
    string input_initial ;
    string query_initial ;   
    Input*  initial_imgs = NULL ; 
    ifstream query_init;

    if ( argv[13] != NULL ) {
        if(argv[14] != NULL ) {
            input_initial = argv[14];
            if( argv[15] != NULL ) {
                if(argv[16] != NULL ) {
                    query_initial = argv[16];    
                    initial_imgs = new Input(input_initial) ;   
                    ifstream query_init(query_initial, ios::binary | ios::in);
                    if(! query_init.is_open()) {
                        cout << "Failed to read query dataset file with initial dataspace!" << endl;
                        exit(1);
                    }                                
                }
                else {
                    cout<<"No query file for initial dataspace found!Please try again!"<<endl;
                    exit(1);
                }
            }
        }
        else {
            cout<<"No input or query file for initial dataspace found!Please try again!"<<endl;
            exit(1);
        }
    }
    //Read image input dataset
    Input* imgs = new Input(input_file);
    GNN* gnn = NULL;
    MRNG* mrng = NULL;
    if(m == 1) {
        //Create Search Structure
        gnn = new GNN(k,E,R,imgs);
    }
    else if(m == 2) {
        //Create Search Structure
        mrng = new MRNG(l,imgs);        
    }
    else if(m != 3){
        cout<<"m must be one of the following: 1,2,3! Please rerun with correct parameters!";
        exit(1);
    }

    //Sum of Approximation Factor out of all the queries
    double maf = 0;
    //Initialize variables for average query time
    double tAverageApproximate = 0.0;
    double tAverageTrue = 0.0;    
    int queries = 10;
    set <pair<double, int>> candidates; 
    int runs = 0 ;
    string answer;

    do {
        if (runs > 0 || query_file.empty())  {  //If user hasn't passed it as command line argument 
            cout<<"Please give the path to query dataset file!"<<endl;
            cin >> query_file;
        }
        if (runs++ > 0 || output_file.empty())  {  //If user hasn't passed it as command line argument 
            cout<<"Please give the path to output file!"<<endl;
            cin >> output_file;
        } 

        ofstream outFile(output_file);
        if (!outFile.is_open()) {
            cerr << "Unable to open the output file."<<endl;
            exit(1);
        }  
        if(m == 1) 
            outFile<<"GNNS Results"<<endl;
        else if(m == 2) 
            outFile<<"MRNG Results"<<endl;    
        else   
            outFile<<"Results for Exhaustive search in New Space"<<endl;

        ifstream query(query_file, ios::binary | ios::in);
        if(!query.is_open()) {
            cout << "Failed to read query dataset file!" << endl;
            exit(1);
        }        
        //Read a small sample of images in the query dataset and perform the algorithms on them
        for(int i = 0; i < queries; i++) {

            Img* query_point = new Img(imgs->get_pxs(),i+1,query);
            //Query in initial dataspace
            Img* init_query = NULL;
            //Estimate the N-Approx Nearest Neighbours keeping track of time 
            const auto start_approx{chrono::steady_clock::now()};

            if(gnn != NULL) 
                candidates = gnn->NearestNeighbour(query_point);
            else if(mrng != NULL)
                candidates = mrng->NearestNeighbour(query_point);
            else
                candidates = imgs->N_Exact(query_point);

            const auto end_approx{chrono::steady_clock::now()};
            chrono::duration<double> t_approx{end_approx - start_approx};  
            tAverageApproximate += t_approx.count();

            //Estimate the N-Exact Nearest Neighbours keeping track of time 
            const auto start_exact{chrono::steady_clock::now()};
            set <pair<double, int>> N_exact;

            //Exact Neighbours' distance should be drawn by initial dataspace
            if(initial_imgs != NULL) {
                init_query = new Img(initial_imgs->get_pxs(),i+1,query_init);
                N_exact = initial_imgs->N_Exact(init_query);
            }
            else {
                N_exact = imgs->N_Exact(query_point);
            }

            const auto end_exact{chrono::steady_clock::now()};
            chrono::duration<double> t_exact{end_exact - start_exact};  
            tAverageTrue += t_exact.count();

            //Write N nearest neighbours in output file or as many as exist
            int maxNeighbors = min((int)N_exact.size(),N);
            maxNeighbors = min((int)candidates.size(),maxNeighbors);
            outFile<<"Query: "<<i+1<<endl; 

            auto approx = candidates.begin();
            auto exact = N_exact.begin();
            double approx_dist = approx->first;

            if(initial_imgs != NULL) {
                int img_num = approx->second;
                Img* init_p = initial_imgs->get_image(img_num);
                approx_dist = init_p->euclideanDistance(init_query);
                delete init_query;
            }

            //Update MAF (we estimate MAF only from the first neighbour)
            double approx_factor = approx_dist / exact->first;
            maf += approx_factor;

            //Iterate through sets and write in output file
            for (int i = 0; i < maxNeighbors; i++) { 
                outFile<<"Nearest Neighbour-" << i + 1 << " :" << approx->second <<endl<< "distanceApproximate: <double> " << approx->first <<endl<< "distanceTrue: <double> "<< exact->first<<endl;
                approx++;
                exact++;
            }
            
            delete query_point;
        }  
        //Calculate average distances
        tAverageApproximate /= queries;
        tAverageTrue /= queries;   
        outFile<<"\ntAverageApproximate: <double> "<<tAverageApproximate<<" sec."<<endl<<"tTrue: <double> "<<tAverageTrue<<" sec."<<endl<<endl<<"MAF: <double> [Mean Approximation Factor] "<<maf/queries<<endl;
        outFile.close();

        do {
            cout<<"Would you like to continue execution for a different query dataset? Please enter y / N !"<<endl;
            cin >> answer;
        }while(answer != "y" && answer != "N");
        
    }while(answer == "y");
      
    delete gnn;
    delete mrng;
    delete imgs;
    if(initial_imgs != NULL) {
        query_init.close();
        delete initial_imgs;
    }
    return 0;
}