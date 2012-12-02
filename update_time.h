#ifndef UPDATE_TIME_H
#define UPDATE_TIME_H

#include "docs_words.h"

class UpdateTime {
public:
    virtual bool operator() (int document, int word_index,  const DocsWords &) = 0;
};

class update_every : public UpdateTime {
private:
    int every;
    int current;
public:
    update_every(int);
    bool operator() (int document, int word_index, const DocsWords &);
};

#endif
