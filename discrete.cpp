#include "discrete.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
//#include <vector>
int Discrete::size() const {
    return static_cast<int>(probability.size());
}

Discrete::Discrete(int size):
    probability(size, 0), counter(size, 0), sum(0) {
}

bool Discrete::inc(int i, double delta) {
    if (delta != delta) {
            std::cout << "*** ERROR: inc -> delta. delta =" << delta  << std::endl;
            return false;
    }
    sum += delta;
    counter[i] += delta;
    return true;
}

void Discrete::add_prob(int index, double delta) {
    probability[index] += delta;
}

double Discrete::get_counter(int index) const {
    return counter[index];
}

// void Discrete::set_counter(int index, double c) {
//     counter[i] = c;
// }

void Discrete::update(double param_zero, const std::vector<double> & parameters) {
    /*if (sum != sum) {
            std::cout << "*** ERROR: upd -> sum. sum =" << sum  << std::endl;
            exit(-8);
    }*/
    double updated_sum = 0;
    for (int i = 0; i < size(); ++i) {
        probability[i] = (counter[i] + parameters[i]) / (sum + param_zero);
        updated_sum += counter[i];
        if (probability[i] != probability[i]) {
            std::cout << "*** ERROR: upd. sum =" << sum << "<<i = " << i << std::endl;
            exit(-9);
        }
    }
    sum = updated_sum;
}

double Discrete::operator() (int index) const {
    return probability[index];
}

std::vector<Pair> Discrete::top(int number) const {
    std::vector<Pair> prob_copy;
    for (int i = 0; i < size(); ++i) {
        prob_copy.push_back(Pair(i, probability[i]));
    };
    std::sort(prob_copy.begin(), prob_copy.end());
    std::vector<Pair> result;
//    for (int i = size() - 1, j = 0; ((i >= 0) && (j < number)); --i, ++j){
//        result.push_back(prob_copy[i]);
//    };
    for (int i = size() - 1; i >= std::max(0, size() - number); --i){
        result.push_back(prob_copy[i]);
    };

    return result;
}
