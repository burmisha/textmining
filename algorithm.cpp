#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "algorithm.h"
#include "count_Z.h"
#include "func.h"

double perform_algorithm(const DocsWords & docs_words,
                        PhiTheta & phi_theta,
                        Hidden_Collection & hidden_collection,
                        Prepare & prepare,
                        GetDelta & get_delta,
                        UpdateTime & update_time,
                        Stabilized & stabilized, double gamma) {
    clock_t zero_time = clock();
    int iteration = 0;
    do {
        for (int d = 0; d < docs_words.docs_number(); ++d) {
            double & nu = hidden_collection.doc[d].nu;
            double new_nu = 0;
            int times = 0;
            if (gamma > 0.0001) {   // magic number
                times = 1;          // CHANGE HERE
            } else {
                times = 1;
                gamma = 0;
            }
            for (int j = 0; j < times; ++j) {
                for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
                    Hidden_Word & word = hidden_collection.doc[d].word[w];
                    const int word_id = docs_words.word_id(d, w);
                    const int n_dw = docs_words.word_counter(d, w);
                    const double Z = count_Z(d, word_id, word, phi_theta);

                    for (size_t i = 0; i < word.topic.size(); ++i) {
                        std::pair<int, double> & topic =  word.topic[i];
                        prepare(d, w, topic, phi_theta);
                        double delta = get_delta(d, word_id, n_dw, i, word, phi_theta, Z);
                        delta = (delta != delta) ? 0 : delta; // silly protection from errors
                        topic.second += delta;
                        phi_theta.inc(d, word_id, topic.first, delta);
                    }
                    if (update_time(d, w, docs_words)) {
                        phi_theta.update();
                    }

                    new_nu += docs_words.word_counter(d, w) * word.pi / Z;
                    double temp_pi = 1 * word.pi + n_dw * gamma / nu - Z; // LOOK at 2
                    //word.pi = my_pos((temp_pi != temp_pi) ? 0 : temp_pi);
                    word.pi = my_pos(temp_pi);
                }
                nu = new_nu;
                double wws = 0;
                for (int ww = 0; ww < docs_words.unique_words_number(d); ++ww) {
                    wws += hidden_collection.doc[d].word[ww].pi;
                }
                /*for (int ww = 0; ww < docs_words.unique_words_number(d); ++ww) {
                    hidden_collection.doc[d].word[ww].pi *= gamma/wws;
                }
                wws = 0;
                for (int ww = 0; ww < docs_words.unique_words_number(d); ++ww) {
                    wws += hidden_collection.doc[d].word[ww].pi;
                }*/
                std::cout << " " << wws / gamma << " " << nu
                            << " " << docs_words.unique_words_number(d)
                            << " " << docs_words.total_words_number(d)
                            << " | ";
            }
            if ((d % 20) == 0) { std::cout << "\r" << "d" << d << " of "<< docs_words.docs_number(); }
        }
        std::cout << "\r" <<"Iteration made: " << ++iteration;
    } while (!stabilized(phi_theta));
    std::cout << std::endl;
    return (float)(clock() - zero_time) / CLOCKS_PER_SEC;
}
