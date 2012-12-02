#include <algorithm>
#include <iostream>
#include "discrete.h"
#include "func.h"

int Discrete::size() const {
    return static_cast<int>(probability.size());
}

Discrete::Discrete(int size):
    probability(size, 0), counter(size, 0), sum(1) {
}

void Discrete::inc(int i, double delta,double gamma) {
    sum -= my_pos(counter[i] + gamma - 1);
    counter[i] += delta;
    sum += my_pos(counter[i] + gamma - 1);
}

void Discrete::add_prob(int index, double delta) {
    probability[index] += delta;
}

double Discrete::get_counter(int index) const {
    return counter[index];
}

void Discrete::update(double param_zero, const std::vector<double> & parameters) {
    double updated_sum = 0;
    for (int i = 0; i < size(); ++i) {
        probability[i] = my_pos(counter[i] + parameters[i] - 1) / sum;
        updated_sum += counter[i];
    }
    sum = updated_sum;
}

void Discrete::update() {
    double updated_sum = 0;
    for (int i = 0; i < size(); ++i) {
        probability[i] = counter[i] / sum;
        updated_sum += counter[i];
    }
    sum = updated_sum;
}

void Discrete::regularize(double added_sum) {
    double add = added_sum / size();
    for (int i = 0; i < size(); ++i) {
        probability[i] = (probability[i] + add) / (1 + added_sum);
    }
}

double Discrete::operator() (int index) const {
    return probability[index];
}

std::vector<std::pair<int, double> > Discrete::top(int number) const {
    std::vector<std::pair<double, int> > prob_copy;
    for (int i = 0; i < size(); ++i) {
        prob_copy.push_back(std::pair<double, int>( - probability[i], i));
    };
    std::sort(prob_copy.begin(), prob_copy.end());
    std::vector<std::pair<int, double> > result;
    for (int i = 0; i < std::min(number, size()); ++i) {
        result.push_back(std::pair<int, double>(prob_copy[i].second, - prob_copy[i].first));
    };
    return result;
}
