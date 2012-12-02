#include "pair.h"

Pair::Pair(int id, double counter) :
    id(id),
    counter(counter) {
};

Pair::Pair():
    id(-1),
    counter(0) { // for impossible situations
};

std::ostream & operator << (std::ostream & out, const Pair & pair) {
    return (out << "(" << pair.id << "; " << pair.counter << ") ");
}

bool Pair::operator < (const Pair & second_pair) const {
    return (this->counter < second_pair.counter);
}
