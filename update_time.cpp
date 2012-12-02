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
