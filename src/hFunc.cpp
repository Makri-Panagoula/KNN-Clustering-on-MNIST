#include "../headers/hFunc.h"

using namespace std;

hFunc::hFunc(int w, int d) {
    static default_random_engine v_generator;
    normal_distribution <double> distribution(0.0, 1.0);

    vector<double> v(d);
    for (int i = 0; i < d; i++) {
        double number = distribution(v_generator);
        v[i] = number;
    }
    this->v = v;
    this->w = w;
}

unsigned int hFunc:: h(vector<unsigned char> p) {

    int dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);

    if (dot_prod < 0) {
        //A vector's magnitude is the square root of its inner product with itself
        int sum_squares = inner_product( p.begin(), p.end(), p.begin(), 0 );
        double norm_p = sqrt(sum_squares);        
        transform(p.begin(), p.end(), p.begin(), [norm_p](unsigned char &c){ return c/norm_p; });
        //Repeat for v
        sum_squares = inner_product( v.begin(), v.end(), v.begin(), 0 );
        double norm_v = sqrt(norm_v);          
        transform(v.begin(), v.end(), v.begin(), [norm_v](double &c){ return c/norm_v; });
        //Estimate new dot product
        dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);
    }

    static default_random_engine t_generator;
    uniform_real_distribution<double> distribution(0.0,this->w);
    double t = distribution(t_generator);  
      
    unsigned int result = (dot_prod + t) / this->w;
    return result;
}