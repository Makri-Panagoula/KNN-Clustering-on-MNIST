#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include "headers/cube.h"

using namespace std;

int main (int argc, char* argv[]) {

    string input_file = argv[2] ;
    string query_file ;

    //Query file is an optional command line argument
    if ( strcmp(argv[4],"–k") ) {
        query_file = argv[4] ;
        argv++;                     //Next character will be "-k" => skip reading it , move into next one 
    }

    //Dimension upon which data will be projected
    int k = 14 ;                       
    //If there is a value after "-k",update k with that
    if ( strcmp(argv[5],"-M") ) {
        k = atoi(argv[5]) ;
        argv++;                     
    }
    
    //Maximum number of datapoints we will consider i the algorithms
    int M = 10 ;                       //Default Value
    if ( strcmp(argv[6],"-prοbes") ) {
        M = atoi(argv[6]) ;
        argv++;                     
    }

    //Maximum verices in the hypercube to be checked
    int probes = 2 ;                       //Default Value
    if ( strcmp(argv[7],"-o")) {
        probes = atoi(argv[7]) ;
        argv++;                     
    }    

    //Output file is optional command line argument
    string output_file ;
    if ( strcmp(argv[8],"-Ν")) {
        output_file = argv[8] ;
        argv++;                     
    }  

    //Number of Nearest Neighbours to be found
    int N = 1 ;                       //Default Value
    if ( strcmp(argv[9],"-R")) {
        N = atoi(argv[9]) ;
        argv++;                     
    }  

    //Radius Size
    int R = 10000 ;                       //Default Value
    if ( argv[10] != NULL ) 
        R = atoi(argv[10]);
    
    //Read image input dataset
    Input* imgs = new Input(input_file);
    //Create Search Structure
    Cube* cube = new Cube(k,M,probes,imgs);

    int runs = 0 ;
    string answer;
    int queries = 1000;
    
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
            exit(1);
        }        
        //Read a small sample of images in the query dataset and perform the algorithms on them
        for(int i = 0; i < queries; i++) {
            Img* query_point = new Img(imgs->get_pxs(),i+1,query);
            cube->queryNeighbours(query_point,N,output_file,R);
            delete query_point;
        }    

        do {
            cout<<"Would you like to continue execution for a different query dataset? Please enter y / N !"<<endl;
            cin >> answer;
        }while(answer != "y" && answer != "N");
        
    }while(answer == "y");
  
    ofstream outFile(output_file , ios::app);
    outFile<<"tAverageApproximate: <double> "<<cube->total_approx()/queries<<" sec."<<endl<<"tTrue: <double> "<<cube->total_true()/queries<<" sec."<<endl<<endl<<"MAF: <double> [Mean Approximation Factor] "<<cube->get_maf()/queries<<endl;
    outFile.close();    
    delete imgs;
    delete cube;
    return 0;
}