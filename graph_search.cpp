#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <sstream>
#include <map>
#include "headers/GNN.h"

using namespace std;

int main (int argc, char* argv[]) {

    string input_file = argv[2] ;
    string query_file ;

    //Query file is an optional command line argument
    if ( strcmp(argv[4],"–k") ) {
        query_file = argv[4] ;
        argv++;                     //Next character will be "-k" => skip reading it , move into next one 
    }

    //K-Nearest neighbours in Graph
    int k = 50 ;                       
    //If there is a value after "-k",update k with that
    if ( strcmp(argv[5],"-E") ) {
        k = atoi(argv[5]) ;
        argv++;                     
    }
    
    //Number of Extensions
    int E = 30 ;                       //Default Value
    if ( strcmp(argv[6],"-R") ) {
        E = atoi(argv[6]) ;
        argv++;                     
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

    //Method to use (1 for GNNS, 2 for MRNG)
    int m = atoi(argv[10]) ;                       //Default Value

    //Output file is optional command line argument
    string output_file ;
    if (argv[12] != NULL) {
        output_file = argv[12] ;
    }  

    //Read image input dataset
    Input* imgs = new Input(input_file);

    //Create Search Structure
    GNN* gnn = new GNN(k,E,R,imgs);
    int runs = 0 ;
    string answer;
    do {
        if ( runs > 0 || query_file.empty())  {  //If user hasn't passed it as command line argument 
            cout<<"Please give the path to query dataset file !"<<endl;
            cin >> query_file;
        }
        if ( runs++ > 0 || output_file.empty())  {  //If user hasn't passed it as command line argument 
            cout<<"Please give the path to output file !"<<endl;
            cin >> output_file;
        }    
        ifstream query(query_file, ios::binary | ios::in);
        if(! query.is_open()) {
            cout << "Failed to read query dataset file!" << endl;
            exit;
        }        
        //Read a small sample of images in the query dataset and perform the algorithms on them
        for(int i = 0; i < 2; i++) {
            Img* query_point = new Img(imgs->get_pxs(),i+1,query);
            gnn->NearestNeighbour(query_point,N,output_file);
            delete query_point;
        }   
        
        do {
            cout<<"Would you like to continue execution for a different query dataset? Please enter y / N !"<<endl;
            cin >> answer;
        }while(answer != "y" && answer != "N");
        
    }while(answer == "y");

    delete gnn;
    delete imgs;
    return 0;
}