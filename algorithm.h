#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <set>
#include <iostream>
#include "docs_words.h"
#include "discrete.h"
#include "distance.h"

class Algorithm {
protected:
    DocsWords docsWords;
    std::vector<Discrete> Phi;
    std::vector<Discrete> Theta;
    std::vector<std::vector<std::vector<Pair> > > HiddenNumbers;

    std::set<int> updated_phi;
    std::set<int> updated_theta;

    const double Phi_zero;
    const std::vector<double> PhiParam;
    const double Theta_zero;
    const std::vector<double> ThetaParam;

    explicit Algorithm(const DocsWords &,
                       int topic_number,
                       double Phi_zero,     const std::vector<double> & PhiParam,
                       double Theta_zero,   const std::vector<double> & ThetaParam,
                       const Distance & distance);
    int topic_number() const;

private:
    int stab_iteration;
    int upd_iteration;

    bool update_needed();
    bool not_stabilised();

    //bool end_of_collection() const;
    bool get_next_word(int & doc, int & word) const;
    void update_if_needed();

    virtual void prepareDocWord(int document, int word) = 0;
    virtual void prepareTopic(int document, int word, int topic) = 0;
    virtual double getDelta(int document, int word, int topic) const = 0;

public:
    void perform();
    friend std::ostream & operator << (std::ostream &, const Algorithm &);
    std::ostream & print(std::ostream &, const Dictionary &) const;
};

#endif
