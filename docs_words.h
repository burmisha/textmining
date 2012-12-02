#ifndef DOCS_WORDS_H
#define DOCS_WORDS_H
#include <vector>
#include "dictionary.h"

struct Pair {
    int id;
    double counter;
    Pair(int topic, double number);
    Pair();
};

class DocsWords {
private:
    std::vector<std::vector<Pair> > docsWords;
    int wordNumber;
public:
    DocsWords(const Dictionary & , FileHandler);
    int docsNumber() const;
    int wordsNumber() const;
    int wordsNumber(int document) const;
    int word(int document, int wordIdx) const;
    int counter(int document, int wordIdx) const;
};

#endif
