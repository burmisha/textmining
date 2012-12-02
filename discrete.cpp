#include "discrete.h"
//#include <vector>
int Discrete::size() const {
    return static_cast<int>(parameters.size()) - 1;
}

Discrete::Discrete(const std::vector<double> & parameters):
    parameters(parameters),
    probability(parameters.size() - 1, 0),
    counter(parameters.size() - 1, 0),
    sum(0) {
}

void Discrete::inc(int i, double delta) {
    sum += delta;
    counter[i] += delta;
}

void Discrete::update() {
    for (int i = 0; i < size(); ++i) {
        probability[i] = (counter[i] + parameters[i + 1]) / (sum + parameters[0]);
    }
}

double Discrete::operator() (int index) const {
    return probability[index];
}
