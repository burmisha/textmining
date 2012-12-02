#ifndef PREPARE_H
#define PREPARE_H
#include "phi_theta.h"

class Prepare {
public:
    virtual void operator()(int document, int word_id, std::pair<int, double> & topic,
                            PhiTheta & phi_theta) = 0;
};



class PLSA_prepare : public Prepare {
public:
    void operator()(int document, int word_id, std::pair<int, double> & topic,
                    PhiTheta & phi_theta);
};

class LDA_prepare : public Prepare {
public:
    void operator()(int document, int word_id, std::pair<int, double> & topic,
                    PhiTheta & phi_theta);
};

#endif
