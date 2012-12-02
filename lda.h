#ifndef LDA_H
#define LDA_H

#include "algorithm.h"

class LDA : public Algorithm {
protected:

    virtual void prepareDocWord(int document, int word);
    virtual void prepareTopic(int document, int word, int topic);
    virtual double getDelta(int document, int word, int topic) const;
public:
    explicit LDA(const DocsWords &,
                 int topic_number,
                 double Phi_zero, 	const std::vector<double> & PhiParam,
                 double Theta_zero, const std::vector<double> & ThetaParam,
                 const Distance & distance);
};

#endif

