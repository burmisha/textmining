#ifndef PLSA_H
#define PLSA_H

#include "algorithm.h"

class PLSA : public Algorithm {
protected:
    double Z;
    virtual void prepareDocWord(int document, int word);
    virtual void prepareTopic(int document, int word, int topic);
    virtual double getDelta(int document, int word, int topic) const;
public:
    explicit PLSA(const DocsWords &, int topic_number);
};

#endif
