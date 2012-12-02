#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "file_handler.h"
#include <string>

class Dictionary {
public:
    virtual int getNumber(std::string const &) const  = 0;
    virtual std::string getWord(int) const = 0;
    virtual bool add (std::string const &) = 0;
    virtual size_t wordNumber() const = 0;
    // virtual void print() const = 0;
    Dictionary() {};
    //virtual bool dump (void) {};
    //virtual bool load (void) {};
    void build(FileHandler);
};

int checkWord(const std::string & word);

#endif

