#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "docs_words.h"
#include "phi_theta.h"
#include "hidden.h"
#include "get_delta.h"
#include "stabilized.h"
#include "update_time.h"
#include "prepare.h"

double perform_algorithm(const DocsWords & ,
                       PhiTheta & ,
                       hidden_type & ,
                       Prepare & ,
                       GetDelta & ,
                       UpdateTime & ,
                       Stabilized & );
#endif
