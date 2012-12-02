#ifndef DISTANCE_H
#define DISTANCE_H

#include "discrete.h"

class Distance {
public:
    virtual double operator()(const Discrete & first,
                              const Discrete & second) const = 0;
};

class KullbackLeibler : public Distance {
public:
    virtual double operator()(const Discrete & first,
                              const Discrete & second) const ;
};

class Chi_squared : public Distance {
public:
    virtual double operator()(const Discrete & first,
                              const Discrete & second) const ;
};

class Hellinger : public Distance {
public:
    virtual double operator()(const Discrete & first,
                              const Discrete & second) const ;
};

#endif
