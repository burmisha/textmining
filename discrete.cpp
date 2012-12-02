#include "discrete.h"
#include <algorithm>
//#include <vector>
int Discrete::size() const {
    return static_cast<int>(parameters.size()) - 1;
}

Discrete::Discrete(const std::vector<double> & parameters):
    parameters(parameters),
    probability(static_cast<int>(parameters.size()) - 1, 0),
    counter(static_cast<int>(parameters.size()) - 1, 0),
    sum(0) {
}

void Discrete::inc(int i, double delta) {
    sum += delta;
    counter[i] += delta;
}

// void Discrete::set_counter(int index, double c) {
//     counter[i] = c;
// }

void Discrete::update(double multiplier) {
    for (int i = 0; i < size(); ++i) {
        probability[i] = (counter[i] + multiplier * parameters[i + 1]) 
                         / (sum      + multiplier * parameters[0]);
    }
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
    for (int i = size() - 1, j = 0; ((i >= 0) && (j < number)); --i, ++j){
        result.push_back(prob_copy[i]);
    };
    return result;
}
