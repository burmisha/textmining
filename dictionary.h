#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "file_handler.h"
#include <string>

class Dictionary {
public:
    Dictionary() {};
    void add_docs(FileHandler);
    virtual int id(std::string const &) const  = 0;
    virtual std::string word(int) const = 0;
    virtual bool add (std::string const &) = 0;
    virtual int size() const = 0;
};

bool good_word(const std::string & word);

#endif

