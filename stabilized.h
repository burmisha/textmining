#ifndef STABILIZED_H
#define STABILIZED_H

#include "phi_theta.h"

class Stabilized {
public:
    virtual bool operator()(const PhiTheta &) = 0;
};

class stabilized_after : public Stabilized {
private:
    int after;
    int current;
public:
    stabilized_after(int after);
    bool operator()(const PhiTheta &);
};

#endif
