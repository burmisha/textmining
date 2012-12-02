#include "hidden.h"
#include <algorithm>
#include "func.h"

Hidden_Collection PLSA_hidden (const DocsWords & docs_words, int topic_number) {
    Hidden_Collection hidden;
    std::vector<std::pair<int, double> > plain;
    for (int i = 0; i < static_cast<int>(topic_number); ++i) {
        plain.push_back(std::pair<int, double>(i, double()));
    }
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        Hidden_Word hw;
        hw.topic = plain;
        Hidden_Document hd;
        hd.word = std::vector<Hidden_Word>(docs_words.unique_words_number(d), hw);
        hidden.doc.push_back(hd);
    }
    return hidden;
}

Hidden_Collection PLSA_robust (const DocsWords & docs_words, int topic_number, double gamma) {
    Hidden_Collection hidden;
    std::vector<std::pair<int, double> > plain;
    for (int i = 0; i < static_cast<int>(topic_number); ++i) {
        plain.push_back(std::pair<int, double>(i, double()));
    }
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        Hidden_Document hd;
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            Hidden_Word hw;
            hw.topic = plain;
            hw.pi = gamma * docs_words.word_counter(d, w) / docs_words.total_words_number(d);
            hd.word.push_back(hw); // TODO: (-1, 0) --- test
        }
        hd.nu = gamma * docs_words.total_words_number(d);
        hidden.doc.push_back(hd);
    }
    return hidden;
}

Hidden_Collection LDA_hidden_limited(const DocsWords & docs_words, int max) {
    Hidden_Collection hidden;
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        Hidden_Document hd;
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            Hidden_Word hw;
            hw.topic = std::vector<std::pair<int, double> >(
                                std::min(docs_words.word_counter(d, w), max), std::pair<int, double>(0, 0));
            hd.word.push_back(hw); // TODO: (-1, 0) --- test
        }
        hidden.doc.push_back(hd);
    }
    return hidden;
}

Hidden_Collection LDA_hidden_multiplicative(const DocsWords & docs_words, double gamma) {
    Hidden_Collection hidden;
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        Hidden_Document hd;
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            Hidden_Word hw;
            hw.topic = std::vector<std::pair<int, double> >(
                                my_round(docs_words.word_counter(d, w) * gamma), std::pair<int, double>(0, 0));
            hd.word.push_back(hw); // TODO: (-1, 0) --- test
        }
        hidden.doc.push_back(hd);
    }
    return hidden;
}

