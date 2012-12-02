#ifndef PAIR_H
#define PAIR_H
#include <iostream>
struct Pair {
    int id;
    double counter;
    Pair();
    Pair(int topic, double number);
    friend std::ostream & operator << (std::ostream &, const Pair &);
    bool operator < (const Pair & ) const;
};

#endif

