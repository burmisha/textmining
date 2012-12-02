#include <math.h>
#include "distance.h"

double KullbackLeibler::operator() (const Discrete & first,
                                    const Discrete & second) const {
    double distance = 0;
    for (int i = 0; i < first.size(); ++i) {
        if ((first(i) > 0) && (second(i) > 0)) {
            distance += first(i)
                        * log(first(i) / second(i));
        }
    }
    return distance;
}

double Chi_squared::operator() (const Discrete & first,
                                const Discrete & second) const  {
    double distance = 0;
    for (int i = 0; i < first.size(); ++i) {
        if (second(i) > 0) {
            distance += pow(second(i) - first(i), 2) / second(i);
        }
    }
    return distance;
}

double Hellinger::operator() (const Discrete & first,
                              const Discrete & second) const  {
    double distance = 0;
    for (int i = 0; i < first.size(); ++i) {
        if ((first(i) >= 0) && (second(i) >= 0)) {
            distance += pow(sqrt(second(i)) - sqrt(first(i)), 2);
        }
    }
    return distance / 2;
}
