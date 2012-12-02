#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <locale.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::ostream;
using std::ifstream;


class Dictionary {
    public:
        virtual int getNumber(string const &) const  = 0;
        virtual string getWord(int) const = 0;
        virtual bool add (string const &) = 0;
		virtual size_t WordNumber() const = 0;
		virtual void print() const = 0;
		Dictionary() {};
		//virtual bool dump (void) {};
		//virtual bool load (void) {};
};




int checkWord(const string & word);
void BuildDictionary(Dictionary & Dict);

#endif
