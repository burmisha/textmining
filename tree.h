#ifndef TREE_H
#define TREE_H
#include "header.h"
struct wordID {
	string word;
	int id;
};

typedef struct nodeStruct {
	nodeStruct * parent;
	nodeStruct * left;
	nodeStruct * right;
	wordID * value;
	int h;
} node;

class tree : public Dictionary {
	private:
		node * root;
		int TotalWordNumber;
		vector<string> WordByID;
	public:
		tree();
		void print() const;
		bool add(string const &) ;
		int getNumber(string const &) const;
        string getWord(int) const;
		size_t WordNumber() const;
};

#endif
