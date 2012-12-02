#ifndef HIDDEN_H
#define HIDDEN_H
#include <vector>
#include <utility>
#include "docs_words.h"

struct Hidden_Word {
	std::vector<std::pair<int, double> > topic;
	double pi; // UNUSUAL $$\hat \pi_{dw} = \gamma * \pi_{dw}$$
};

struct Hidden_Document {
	std::vector<Hidden_Word> word;
	double nu; // USUAL $$ \nu_d $$
};

struct Hidden_Collection {
	std::vector<Hidden_Document> doc;
};

Hidden_Collection PLSA_hidden(const DocsWords &, int topic_number);
Hidden_Collection PLSA_robust(const DocsWords &, int topic_number, double gamma);
Hidden_Collection LDA_hidden_limited(const DocsWords &, int max);
Hidden_Collection LDA_hidden_multiplicative(const DocsWords &, double);

#endif
