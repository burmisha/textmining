#include "hidden.h"


hidden_type PLSA_hidden (const DocsWords & docs_words, int topic_number) {
    hidden_type hidden;
    std::vector<std::pair<int, double> > plain;
    for (int i = 0; i < static_cast<int>(topic_number); ++i) {
        plain.push_back(std::pair<int, double>(i, double()));
    }
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        hidden.push_back(std::vector<std::vector<std::pair<int, double> > >(docs_words.unique_words_number(d), plain));
    }
    return hidden;
}

hidden_type LDA_hidden(const DocsWords & docs_words) {
    hidden_type hidden;
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        std::vector<std::vector<std::pair<int, double> > > document;
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            document.push_back(std::vector<std::pair<int, double> >(docs_words.word_counter(d, w), std::pair<int, double>(0, 0))); // TODO: (-1, 0) --- test
        }
        hidden.push_back(document);
    }
    return hidden;
}


