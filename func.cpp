#include "func.h"
#include "math.h"

int my_round(double d) {
    if (d > 0) {
        return floor(d + 0.5);
    } else if (d < 0) {
        return -floor(-d + 0.5);
    } else {
        return 0;
    }
}

double my_abs(double d) {
    if (d > 0) {
        return d;
    } else if (d < 0) {
        return - d;
    } else {
        return 0;
    }
}

double my_pos(double d) {
    return (d > 0) ? d : 0;
}
