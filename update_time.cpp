#include "update_time.h"

update_every::update_every(int every) : every(every), current(0)  {
}

bool update_every::operator()(int document, int word_index, const DocsWords &) {
    ++current;
    if (current >= every){
        current = 0;
        return true;
    } else {
        return false;
    }
}

update_after_document::update_after_document() {
}

bool update_after_document::operator()(int document, int word_index, const DocsWords & docs_words) {
    return ((word_index + 1) == docs_words.unique_words_number(document));
}

update_after_collection::update_after_collection() {
}

bool update_after_collection::operator()(int document, int word_index, const DocsWords & docs_words) {
    return ((document + 1) == docs_words.docs_number());
}

