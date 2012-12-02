#ifndef QUALITY_H
#define QUALITY_H

#include "docs_words.h"
#include "phi_theta.h"
#include "distance.h"

double count_quality(const DocsWords &, const PhiTheta &, const Distance &);

#endif
