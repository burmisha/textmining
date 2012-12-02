#include "docs_words.h"
#include <time.h>
#include <algorithm>
#include <iostream>

int DocsWords::docs_number() const {
    return static_cast<int>(docs_words.size());
}

int DocsWords::unique_words_number(int document) const {
    return static_cast<int>(docs_words[document].size());
}

int DocsWords::unique_words_number() const {
    return dictionary_words_number;
}

int DocsWords::word_id(int document, int word_idx) const {
    return docs_words[document][word_idx].first;
}

int DocsWords::word_counter(int document, int word_idx) const {
    return docs_words[document][word_idx].second;
}

DocsWords::DocsWords(const Dictionary & dictionary, FileHandler file_handler):
    dictionary_words_number(dictionary.wordNumber()) {
    file_handler.drop();
    clock_t zeroTime = clock();
    while (file_handler.goto_next()) {
        clock_t firstTime = clock();
        std::ifstream file(file_handler.filename().c_str());

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

        std::vector<std::pair<int, int> > docWordElements;
        size_t word_idx = 0, times = 0;
        while ((word_idx + 1) < document.size()) {
            if (document[word_idx] != document[word_idx + 1]) {
                docWordElements.push_back(std::pair<int, int>(document[word_idx], word_idx - times + 1));
                times = word_idx + 1;
            }
            ++word_idx;
        }
        docWordElements.push_back(std::pair<int, int>(document[word_idx], word_idx - times + 1));
        if (docWordElements.empty()) {
            std::cout << "WARNING: empty document. Press Ctrl-C, delete document from collection, rename, run program again" << std::endl;
        } else {
            docs_words.push_back(docWordElements);
        }
        std::cout << "  count words for text " << file_handler.short_filename() << " made in " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << std::endl;
        file.close();
    }
    std::cout << "DOCSWORDS: total time is " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}
