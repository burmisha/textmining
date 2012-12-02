#ifndef DISCRETE_H
#define DISCRETE_H
#include <vector>

class Discrete {
private:
    std::vector<double> parameters;
    std::vector<double> probability;
    std::vector<double> counter;
    double sum;
public:
    int size() const;
    Discrete(const std::vector<double> & parameters);
    void inc(int i, double delta);
    void update();
    double operator() (int index) const;
};

#endif
