#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "algorithm.h"
#include "docs_words.h"

Algorithm::Algorithm(const DocsWords & docsWords,
                     int topic_number,
                     const std::vector<double> & PhiParam,
                     const std::vector<double> & ThetaParam,
                     const Distance & distance):
    Phi(topic_number, Discrete(PhiParam)),
    Theta(docsWords.docs_number(), Discrete(ThetaParam)),
    docsWords(docsWords),
    stab_iteration(0),
    upd_iteration(0) {
        std::vector<double> words_counter(docsWords.unique_words_number(), 0);
        for (int t = 0; t < topic_number; ++t) {
            int d = rand() * docsWords.docs_number() / RAND_MAX;
            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                Phi[t].inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
                words_counter[docsWords.word_id(d, w)] += docsWords.word_counter(d, w);
            }
            Phi[t].update(0);
        }

        const int R = 3 * topic_number;
        for (int r = 0; r < R; ++r) {
            int d = rand() * docsWords.docs_number() / RAND_MAX;
            Discrete words_distribution(std::vector<double>(docsWords.unique_words_number() + 1, 0)); // $$\hat p_d$$
            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                    words_distribution.inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
            }
            int best_topic = 0;
            double min_distance = distance(Phi[0], words_distribution);
            for (int t = 1; t < topic_number; ++t) {
                if (distance(Phi[t], words_distribution) < min_distance) {
                    best_topic = t;
                }     
            }
            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                Phi[best_topic].inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
                words_counter[docsWords.word_id(d, w)] += docsWords.word_counter(d, w);
            } 
            Phi[best_topic].update(0);
        }
}

int Algorithm::topicsNumber() const {
    return Phi.size();
}

bool Algorithm::update_needed() {
    ++upd_iteration;
    return (upd_iteration >= 1000);
}

bool Algorithm::not_stabilised() {
    ++stab_iteration;
    std::cout << "  iteration made: " << stab_iteration << std::endl;
    return stab_iteration < 5;
}

void Algorithm::update_if_needed() {
    if (!update_needed()) {
        return;
    }
    for (size_t i = 0; i < Phi.size(); ++i) {
        Phi[i].update(1);
    }
    for (size_t i = 0; i < Theta.size(); ++i) {
        Theta[i].update(1);
    }
}

bool Algorithm::get_next_word(int & document, int & wordIdx) const {
    if (wordIdx + 1 < docsWords.unique_words_number(document)) {
        ++wordIdx;
    } else {
        wordIdx = 0;
        ++document;
        std::cout << "d#" << document << " ";
    }
    return (document < docsWords.docs_number());
}


void Algorithm::perform() {
    clock_t zeroTime = clock();
    do {
        int d = 0, w = -1;
        while (get_next_word(d, w)) {
            // w is index, it is NOT a real word number
            prepareDocWord(d, w);
            //std::cout << d << " "  << w << std::endl;
            for (size_t i = 0; i < HiddenNumbers[d][w].size(); ++i) {
                prepareTopic(d, w, i);
                int topic = HiddenNumbers[d][w][i].id;
                double delta = getDelta(d,w,topic);
                HiddenNumbers[d][w][i].counter += delta;
                Phi[topic].inc(docsWords.word_id(d, w), delta);
                Theta[d].inc(topic, delta);
            }
            update_if_needed();
        }
    } while (not_stabilised());
    std::cout << "Algorithm PERFORMED. Total time is " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}

std::ostream & operator << (std::ostream & out, const Algorithm & algorithm) {
    out << "Phi  ---  topic -> words" << std::endl;
    for (int topic = 0; topic < static_cast<int>(algorithm.Phi.size()); ++topic) {
        out << "t" << topic << " : ";
        std::vector<Pair> top(algorithm.Phi[topic].top(5));
        for (size_t pair = 0; pair < top.size(); ++pair) {
            out << top[pair];
        }
        out << std::endl;
    }
    out << std::endl;

    out << "Theta  ---  document -> topic" << std::endl;
    for (int topic = 0; topic < static_cast<int>(algorithm.Theta.size()); ++topic) {
        out << "t" << topic << " : ";
        std::vector<Pair> top(algorithm.Theta[topic].top(2));
        for (size_t pair = 0; pair < top.size(); ++pair) {
            out << top[pair];
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}
