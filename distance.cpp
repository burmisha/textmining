#include <math.h>
#include "distance.h"

static const double epsilon = 0.00001;

double KullbackLeibler::operator() (const Discrete & first,
                                    const Discrete & second) const {
    double distance = 0;
    for (int i = 0; i < first.size(); ++i) {
        if ((first(i) > epsilon) && (second(i) > epsilon)) {
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
        if (second(i) > epsilon) {
            distance += pow(second(i) - first(i), 2) / second(i);
        }
    }
    return distance;
}

double Hellinger::operator() (const Discrete & first,
                              const Discrete & second) const  {
    double distance = 0;
    for (int i = 0; i < first.size(); ++i) {
        if ((first(i) >= epsilon) && (second(i) >= epsilon)) {
            distance += pow(sqrt(second(i)) - sqrt(first(i)), 2);
        }
    }
    return distance / 2;
}
