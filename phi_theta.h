#ifndef PHI_THETA_H
#define PHI_THETA_H

#include <vector>
#include <set>
#include "discrete.h"
#include "docs_words.h"
#include "distance.h"

class HyperParameter {
private:
    std::vector<Discrete> LatinLetter;
    const std::vector<double> alpha;
    double alpha_null;
    std::set<int> updated_indexes;
    bool fixed;
public:
    HyperParameter(int size, const std::vector<double> & alpha);
    void inc(int first_idx, int second_idx, double delta);
    void update();
    std::vector<std::vector<std::pair<int, double> > > top(int size) const;
    Discrete & operator[](int);
    const Discrete & operator[](int) const;
    void set_fixed(bool);
    std::vector<double> alpha_parameters() const;
    //HyperParameter same_empty(int new_size);
};

class PhiTheta {
private:
    HyperParameter Phi, Theta;
    PhiTheta(const HyperParameter &, const HyperParameter &);
public:
    void set_phi_fixed(bool);
    void set_theta_fixed(bool);
    PhiTheta(const DocsWords & docs_words, const Distance & distance,
             const std::vector<double> & alpha, const std::vector<double> & beta);
    void update();
    void inc(int document, int word, int topic, double delta);
    double phi(int i, int j) const;
    double theta(int i, int j) const;
    int topic_number() const;
    std::ostream & print(std::ostream & out, const Dictionary & dict) const;
    PhiTheta save_phi(const DocsWords &) const;

};

#endif
