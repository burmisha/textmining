#ifndef DOCS_WORDS_H
#define DOCS_WORDS_H
#include <vector>
#include <utility>
#include "dictionary.h"
#include "file_handler.h"

class DocsWords {
private:
    std::vector<std::vector<std::pair<int, int> > > docs_words;
    std::vector<int> docs_size;
    int dictionary_words_number;
    void add_document(std::vector<int>);
public:
    DocsWords(const Dictionary & , FileHandler);
    DocsWords();
    int docs_number() const;
    int total_words_number(int document) const;
    int unique_words_number() const;  // i.e. dictionary_words_number
    int unique_words_number(int document) const;
    int word_id(int document, int wordIdx) const;
    int word_counter(int document, int wordIdx) const;
    std::pair<DocsWords, DocsWords> build_control(const Dictionary & , FileHandler);
};

#endif
