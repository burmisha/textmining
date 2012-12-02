#include "docs_words.h"
#include "file_handler.h"
#include <time.h>
#include <algorithm>
#include <iostream>

Pair::Pair(int id, double counter) :
    id(id),
    counter(counter) {
};

Pair::Pair():
    id(-1),
    counter(0) { // for impossible situations
};


int DocsWords::docsNumber() const {
    return static_cast<int>(docsWords.size());
}

int DocsWords::wordsNumber(int document) const {
    return static_cast<int>(docsWords[document].size());
}

int DocsWords::wordsNumber() const {
    return wordNumber;
}

int DocsWords::word(int document, int word_idx) const {
    // get global wordID by its position
    return docsWords[document][word_idx].id;
}

int DocsWords::counter(int document, int word_idx) const {
    // get number of words in document
    return docsWords[document][word_idx].counter;
}

DocsWords::DocsWords(const Dictionary & dictionary, FileHandler fileHandler):
    wordNumber(dictionary.wordNumber()) {
    fileHandler.drop();
    clock_t zeroTime = clock();
    while (fileHandler.goto_next()) {
        clock_t firstTime = clock();
        std::ifstream file(fileHandler.filename().c_str());

        std::vector<int> document;
        while (!file.eof()) {
            std::string word;
            file >> word;
            if (dictionary.getNumber(word) >= 0) {
                document.push_back(dictionary.getNumber(word));
            }
        }
        // cout << document;
        std::sort(document.begin(), document.end());

        std::vector<Pair> docWordElements;
        size_t word_idx = 0, times = 0;
        while ((word_idx + 1) < document.size()) {
            if (document[word_idx] != document[word_idx + 1]) {
                docWordElements.push_back(Pair(document[word_idx], word_idx - times + 1));
                times = word_idx + 1;
            }
            ++word_idx;
        }
        docWordElements.push_back(Pair(document[word_idx], word_idx - times + 1));
        if (docWordElements.empty()) {
            std::cout << "WARNING: empty document. Press Ctrl-C, delete document from collection, rename, run program again" << std::endl;
        } else {
            docsWords.push_back(docWordElements);
        }
        std::cout << "  count words for text " << fileHandler.short_filename() << " made in " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << std::endl;
        file.close();
    }
    std::cout << "DOCSWORDS: total time is " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}
