#include "../headers/hFunc.h"
#include <functional>
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


// double hFunc:: magnitude(vector v) {
//     // calculating normalized value
//     double norm_p = 0.0;
//     for(double value : p) {
//         norm_p += value * value;
//     }
//     norm_p = sqrt(norm_p);
//     return norm_p;
// }

int hFunc:: h(vector<unsigned int> p) {

    int dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);

    if (dot_prod < 0) {
        //Normalize p,v
        double norm_p = 0.0;
        for(double value : p) {
            norm_p += value * value;
        }
        norm_p = sqrt(norm_p);        
        vector<double> normalized = accumulate(p.begin(), p.end(), norm_p, divides<int>());
        // v = v / magnitude(v);
        dot_prod = inner_product(p.begin(),p.end(),this->v.begin(),0);
    }

    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,this->w);
    double t = distribution(generator);  
      
    int result = (dot_prod + t) / this->w;
    return result;
}

int hFunc::h(vector<unsigned int>  p) {
    int dot_prod = inner_product(p.begin(), p.end(), this->v.begin(), 0);

    // random t
    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0, this->w);
    double t = distribution(generator);

    int result;

    if (dot_prod < 0) {


        double normalized_result = static_cast<double>(dot_prod) / (this->w * norm_p);
        result = static_cast<int>(normalized_result + t);
    } else {
        result = dot_prod + static_cast<int>(t);
    }

    return result;
}
