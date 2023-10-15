#include "../headers/hFunc.h"

using namespace std;

hFunc::hFunc(int w, int d) {
    default_random_engine generator;
    normal_distribution <double> distribution(0.0, 1.0);

    vector<double> v(d);
    for (int i = 0; i < d; i++) {
        double number = distribution(generator);
        v[i] = number;
    }
    this->v = v;
    this->w = w;
}

int hFunc:: h(vector<unsigned int> p) {

    int dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);

    if (dot_prod < 0) {
        //A vector's magnitude is the square root of its inner product with itself
        int sum_squares = inner_product( p.begin(), p.end(), p.begin(), 0 );
        double norm_p = sqrt(sum_squares);        
        transform(p.begin(), p.end(), p.begin(), [norm_p](unsigned int &c){ return c/norm_p; });
        //Repeat for v
        sum_squares = inner_product( v.begin(), v.end(), v.begin(), 0 );
        double norm_v = sqrt(norm_v);          
        transform(v.begin(), v.end(), v.begin(), [norm_v](double &c){ return c/norm_v; });
        //Estimate new dot product
        dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);
    }

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,this->w);
    double t = distribution(generator);  
      
    unsigned int result = (dot_prod + t) / this->w;
    return result;
}