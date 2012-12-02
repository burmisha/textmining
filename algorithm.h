#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "docs_words.h"
#include "phi_theta.h"
#include "hidden.h"
#include "get_delta.h"
#include "stabilized.h"
#include "update_time.h"
#include "prepare.h"

class Algorithm {
private:
    DocsWords docs_words;
    PhiTheta & phi_theta;
    hidden_type & hidden;
    Prepare & prepare;
    GetDelta & get_delta;
    Stabilized & stabilized;
    UpdateTime & update_time;
public:
    void operator() (void);
    explicit Algorithm(const DocsWords &, PhiTheta &, hidden_type &,
                       Prepare &, GetDelta &, UpdateTime &, Stabilized &);
};

#endif
