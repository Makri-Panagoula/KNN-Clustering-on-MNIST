#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <iostream>
// #include "../headers/hFunc.h"
#include <vector>
#include <numeric>  
#include <cmath> 

using namespace std;

int main (int argc, char* argv[]) {

    string input_file = argv[2] ;
    string query_file ;

    //Query file is an optional command line argument
    if ( strcmp(argv[4],"–k") ) {
        query_file = argv[4] ;
        argv++;                     //Next character will be "-k" => skip reading it , move into next one 
    }

    //Number of h-functions out of which every g function will be constructed
    int k = 4 ;                       
    //If there is a value after "-k",update k with that
    if ( strcmp(argv[5],"-L") ) {
        k = atoi(argv[5]) ;
        argv++;                     
    }
    
    //Number of Hashtables we will use
    int L = 5 ;                       //Default Value
    if ( strcmp(argv[6],"-ο") ) {
        L = atoi(argv[6]) ;
        argv++;                     
    }

    //Output file is optional command line argument
    string output_file ;
    if ( strcmp(argv[7],"-Ν")) {
        output_file = argv[7] ;
        argv++;                     
    }  

    //Number of Nearest Neighbours
    int N = 1 ;                       //Default Value
    if ( strcmp(argv[8],"-R")) {
        N = atoi(argv[8]) ;
        argv++;                     
    }    

    //Radius Size
    int R = 10000 ;                       //Default Value
    if ( argv[9] != NULL ) 
        R = atoi(argv[9]);
    
    //Create Search Structure...  

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

        do {
            cout<<"Would you like to continue execution for a different query dataset? Please enter y / N !"<<endl;
            cin >> answer;
        } while (answer == "y" || answer == "N");
        
    }while(answer == "y");

    return 0;
}