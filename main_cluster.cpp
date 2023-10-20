#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <sstream>
#include "headers/lsh.h"
#include "headers/img.h"
// #include "headers/input.h"
#include <map>

using namespace std;

int main (int argc, char* argv[]) {

    string input_file = argv[2];
    
    //Read cluster parameters from configuration file
    ifstream conf(argv[4]);
    if (!conf.is_open()) {
        cout<< "Failed to open cluster.conf" <<endl;
        exit;
    }     

    //initializing map with default parameters
    map<string, int> parameters = {
        {"number_of_clusters:", 0},
        {"number_of_vector_hash_tables:", 3},
        {"number_of_vector_hash_functions:", 4},
        {"max_number_M_hypercube:", 10},
        {"number_of_hypercube_dimensions:", 3},
        {"number_of_probes:", 2}
    };

    string line;
    string paramName;
    int paramValue;   
    while (getline(conf, line)) {
        istringstream iss(line);
        //Parse string into the parameter name and value
        if (iss>>paramName>>paramValue) 
            parameters[paramName] = paramValue;
    }
    conf.close(); 

    string output_file  = argv[6];
    
    //If complete parameter isn't given as a command argument is by default false
    int complete = 0;               
    if ( strcmp(argv[7],"-m")) {
        complete = 1;
        argv++;                     
    }    


    // //Read image input dataset
    // Input* imgs = new Input(input_file);
    // //Create Search Structure where we are only gonna be saving the centroids
    // //(since we want to find the nearest neighbour out of them and non-centroid point in its iteration)
    // if ( ! strcmp(argv[8],"Classic") ) {

    // }
    // else if(! strcmp(argv[8],"LSH") ) {

    // }
    // else if(! strcmp(argv[8],"Hypercube") ) {

    // }    
    // LSH lsh(L,k,imgs);

    // int runs = 0 ;
    // string answer;
    // do {
    //     if ( runs > 0 || query_file.empty())  {  //If user hasn't passed it as command line argument 
    //         cout<<"Please give the path to query dataset file !"<<endl;
    //         cin >> query_file;
    //     }
    //     if ( runs++ > 0 || output_file.empty())  {  //If user hasn't passed it as command line argument 
    //         cout<<"Please give the path to output file !"<<endl;
    //         cin >> output_file;
    //     }    
    //     ifstream query(query_file, ios::binary | ios::in);
    //     if(! query.is_open()) {
    //         cout << "Failed to read query dataset file!" << endl;
    //         exit;
    //     }        
    //     //Read a small sample of images in the query dataset and perform the algorithms on them
    //     for(int i = 0; i < 2; i++) {
    //         Img* query_point = new Img(imgs->get_pxs(),i+1,query);
    //         lsh.queryNeighbours(query_point,N,output_file,R);
    //         delete query_point;
    //     }   
    //     do {
    //         cout<<"Would you like to continue execution for a different query dataset? Please enter y / N !"<<endl;
    //         cin >> answer;
    //     }while(answer != "y" && answer != "N");
        
    // }while(answer == "y");

    // //Write time metrics into output file
    // ofstream outFile(output_file, ios::app); 
    // outFile<<"tLSH: <double> "<<lsh.get_tLSH()<<" sec."<<endl<<"tTrue: <double> "<<lsh.get_tTrue()<<" sec."<<endl<<endl;
    // outFile.close();
    
    // delete imgs;
    return 0;
}