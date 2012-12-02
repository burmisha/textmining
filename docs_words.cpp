#include "docs_words.h"
#include <time.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

int DocsWords::docs_number() const {
    return static_cast<int>(docs_words.size());
}

int DocsWords::unique_words_number() const {
    return dictionary_words_number;
}

int DocsWords::unique_words_number(int document) const {
    return static_cast<int>(docs_words[document].size());
}

int DocsWords::total_words_number(int document) const {
    return docs_size[document];
}


int DocsWords::word_id(int document, int word_idx) const {
    return docs_words[document][word_idx].first;
}

int DocsWords::word_counter(int document, int word_idx) const {
    return docs_words[document][word_idx].second;
}
void DocsWords::add_document(std::vector<int> document) {
    std::vector<std::pair<int, int> > docWordElements;
    std::sort(document.begin(), document.end());
    size_t word_idx = 0, times = 1;
    while (++word_idx < document.size()) {
        if (document[word_idx - 1] != document[word_idx]) {
            docWordElements.push_back(std::pair<int, int>(document[word_idx - 1], times));
            times = 1;
        } else {
            ++times;
        }
    }
    docWordElements.push_back(std::pair<int, int>(document[word_idx - 1], times));
    if (docWordElements.empty()) {
        std::cout << "WARNING: empty document. Press Ctrl-C, delete document from collection, rename all, run again" << std::endl;
    } else {
        docs_words.push_back(docWordElements);
        docs_size.push_back(static_cast<int>(document.size()));
    }
}

DocsWords::DocsWords() {
};

std::pair<DocsWords, DocsWords> DocsWords::build_control(const Dictionary & dictionary, FileHandler file_handler) {
    DocsWords train, test;
    train.dictionary_words_number = dictionary_words_number;
    test.dictionary_words_number = dictionary_words_number;
    file_handler.drop();
    clock_t zeroTime = clock();
    while (file_handler.goto_next()) {
        clock_t firstTime = clock();
        std::ifstream file(file_handler.filename().c_str());

        std::vector<int> document_for_train, document_for_test;
        while (!file.eof()) {
            std::string word;
            file >> word;
            if (dictionary.id(word) >= 0) {
                if (rand() > (0.5 * RAND_MAX)) {
                    document_for_train.push_back(dictionary.id(word));
                } else {
                    document_for_test.push_back(dictionary.id(word));
                }
            }
        }

        train.add_document(document_for_train);
        test.add_document(document_for_test);

        std::cout << "\r  text \"" << file_handler.short_filename() << "\" added into control in " << (float)(clock() - firstTime) / CLOCKS_PER_SEC;
        file.close();
    }
    std::cout << std::endl << "CONTROL built in " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
    return std::pair<DocsWords, DocsWords>(train, test);
}

DocsWords::DocsWords(const Dictionary & dictionary, FileHandler file_handler):
    dictionary_words_number(dictionary.size()) {
    file_handler.drop();
    clock_t zeroTime = clock();
    while (file_handler.goto_next()) {
        clock_t firstTime = clock();
        std::ifstream file(file_handler.filename().c_str());
        std::vector<int> document;
        while (!file.eof()) {
            std::string word;
            file >> word;
            if (dictionary.id(word) >= 0) {
                document.push_back(dictionary.id(word));
            }
        }
        add_document(document);
        std::cout << "\r  count words for text \"" << file_handler.short_filename() << "\" made in " << (float)(clock() - firstTime) / CLOCKS_PER_SEC;
        file.close();
    }
    std::cout << std::endl << "DOCSWORDS: total time is " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}
