#include "tree.h"
#include <stdlib.h>
#include <iostream>
const int IMPOSSIBLE = -1;

bool Tree::add(std::string const & word)  {
    if(checkWord(word) == 0) {
        return false;
    }
    node * current = root;
    node * parent = root;
    int comparasion = 0;
    while(current != NULL) {
        comparasion = current->value->word.compare(word);
        if(comparasion == 0) {
            return true; // is already in the tree
        }
        parent = current;
        if(comparasion > 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if ((current = (node*)malloc(sizeof(node))) == NULL) {
        std::cout << "Can't malloc memory for new node" << std::endl;
        return false;
    }
    current->value = new wordID;
    current->value->id = TotalWordNumber;
    current->value->word = word;
    if(comparasion > 0) {
        parent->left = current;
    } else {
        parent->right = current;
    }
    current->parent = parent;
    current->left = NULL;
    current->right = NULL;
    //функции балансировки
    WordByID.push_back(word);
    TotalWordNumber += 1;
    // cout << " TWN :" << TotalWordNumber << endl;
    return true;    //  added
};

Tree::Tree():
    TotalWordNumber(0),
    WordByID(0) {
    if((root = (node *) malloc(sizeof(node))) == NULL) {
        std::cout << "Can't malloc memory for new node" << std::endl;
        return; // this is great breeeeeed
    }
    root->value = new wordID;
    root->value->word = std::string("");
    root->value->id = IMPOSSIBLE;   //  addressing it must cause error
    root->left = NULL;
    root->parent = NULL;
    root->right = NULL;
}

int Tree::getNumber(std::string const & word)  const {
    node * current = root;
    while(current != NULL) {
        int comparation = current->value->word.compare(word);
        if(comparation == 0) {
            return current->value->id;
        }
        if(comparation > 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return -1; // no word
}

std::string Tree::getWord(int n) const {
    if((n >= 0) && (n < TotalWordNumber)) {
        return WordByID[n];
    } else {
        std::cout << "ERROR: No such Number!" << std::endl;
        return std::string("");
    }
}

size_t Tree::wordNumber() const {
    return TotalWordNumber;
}

void printTree(node * root) {
    if(root->left != NULL) {
        printTree(root->left);
    }
    std::cout << "|" << root->value->word << std::endl;
    if(root->right != NULL) {
        printTree(root->right);
    }
}

void Tree::print() const {
    printTree(root);
}
