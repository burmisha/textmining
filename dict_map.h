#ifndef DICT_MAP_H
#define DICT_MAP_H

#include "dictionary.h"
#include <map>
#include <vector>

class Map : public Dictionary {
    std::map<std::string, int> word_id_map;
    std::vector<std::string> word_by_id;
public:
    Map();
    bool add(std::string const &) ;
    int id(std::string const &) const;
    std::string word(int) const;
    int size() const;
};

#endif
