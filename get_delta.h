#ifndef GETDELTA_H
#define GETDELTA_H

#include <vector>
#include <utility>
#include "phi_theta.h"
#include "hidden.h"

class GetDelta {
public:
    virtual double operator() (int document,
                               int word_id, int word_counter, int hidden_idx,
                               const Hidden_Word & topics,
                               const PhiTheta & phi_theta,
                               double Z) = 0;
};

class LDA_GetDelta : public GetDelta {
public:
    virtual double operator() (int document,
                               int word_id, int word_counter, int hidden_idx,
                               const Hidden_Word & topics,
                               const PhiTheta & phi_theta,
                               double Z);
};

class PLSA_GetDelta : public GetDelta {
private:
    double Z;
    int ready_word_id;
    int ready_doc;
public:
    virtual double operator() (int document,
                               int word_id, int word_counter, int hidden_idx,
                               const Hidden_Word & topics,
                               const PhiTheta & phi_theta,
                               double Z);
};

#endif
