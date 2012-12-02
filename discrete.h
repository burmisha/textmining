#ifndef DISCRETE_H
#define DISCRETE_H
#include <vector>
#include "pair.h"

class Discrete {
    //friend class Distance;
private:
    // std::vector<double> parameters;
    std::vector<double> probability;
    std::vector<double> counter;
    double sum;
public:
    int size() const;
    Discrete(int size);
    bool inc(int i, double delta);
    void add_prob(int i, double delta);
    double get_counter(int i) const;
    void update(double param_zero, const std::vector<double> & parameters);
    //void regularize(double addend);
    double operator() (int index) const;
    std::vector<Pair> top(int number) const;

    // void set_counter(int index, double counter);
};

#endif
