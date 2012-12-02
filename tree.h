#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>
#include "dictionary.h"

struct wordID {
    std::string word;
    int id;
};

typedef struct nodeStruct {
    nodeStruct * parent;
    nodeStruct * left;
    nodeStruct * right;
    wordID * value;
    int h;
} node;

class Tree : public Dictionary {
private:
    node * root;
    int TotalWordNumber;
    std::vector<std::string > WordByID;
public:
    Tree();
    void print() const;
    bool add(std::string  const &) ;
    int getNumber(std::string  const &) const;
    std::string  getWord(int) const;
    size_t wordNumber() const;
};

#endif
