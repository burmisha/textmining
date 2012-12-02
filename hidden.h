#ifndef HIDDEN_H
#define HIDDEN_H
#include <vector>
#include <utility>
#include "docs_words.h"
typedef std::vector<std::vector<std::vector<std::pair<int, double> > > > hidden_type;

hidden_type PLSA_hidden(const DocsWords &, int topic_number);
hidden_type LDA_hidden(const DocsWords &);

#endif
