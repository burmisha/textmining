#ifndef DISCRETE_H
#define DISCRETE_H
#include <vector>
#include <utility>

class Discrete {
private:
    std::vector<double> probability;
    std::vector<double> counter;
    double sum;
public:
    int size() const;
    Discrete(int size);
    void inc(int i, double delta, double gamma);
    void add_prob(int i, double delta);
    double get_counter(int i) const;
    void update(double alpha_null, const std::vector<double> & alpha);
    void update();
    void regularize(double added_sum);
    double operator() (int index) const;
    std::vector<std::pair<int, double> > top(int number) const;
};

#endif
