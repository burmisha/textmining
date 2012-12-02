#include "stabilized.h"

stabilized_after::stabilized_after(int after) : after(after), current(0)  {
}

bool stabilized_after::operator() (const PhiTheta &) {
    ++current;
    return (current >= after);
}
