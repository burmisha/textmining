#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "algorithm.h"

double perform_algorithm(const DocsWords & docs_words,
                       PhiTheta & phi_theta,
                       hidden_type & hidden,
                       Prepare & prepare,
                       GetDelta & get_delta,
                       UpdateTime & update_time,
                       Stabilized & stabilized) {
    clock_t zero_time = clock();
    int iteration = 0;
    do {
        for (int d = 0; d < docs_words.docs_number(); ++d) {
            for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
                for (size_t i = 0; i < hidden[d][w].size(); ++i) {
                    prepare(d, w, hidden[d][w][i], phi_theta);
                    int topic = hidden[d][w][i].first;
                    double delta = get_delta(d, docs_words.word_id(d, w), docs_words.word_counter(d, w),
                                             i, hidden[d][w], phi_theta);
                    delta = (delta != delta) ? 0 : delta; // silly protection from errors   //std::cout << delta << " ";
                    hidden[d][w][i].second += delta;
                    phi_theta.inc(d, docs_words.word_id(d, w), topic, delta);
                }
                if (update_time(d, w, docs_words)) {
                    phi_theta.update();
                }
                //std::cout << "d" << d << " w "<< w << "\r";
            }
            if ((d % 100) == 0) { std::cout << "\r" << "d" << d << " of "<< docs_words.docs_number(); }
        }
        std::cout << "\r" <<"Iteration made: " << ++iteration;
    } while (!stabilized(phi_theta));
    std::cout << std::endl;
    return (float)(clock() - zero_time) / CLOCKS_PER_SEC;
}
