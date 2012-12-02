#include "algorithm.h"
#include "docs_words.h"
#include <time.h>
#include <iostream>

Algorithm::Algorithm(const DocsWords & docsWords,
                     int topic_number,
                     const std::vector<double> & PhiParam,
                     const std::vector<double> & ThetaParam):
    Phi(topic_number, Discrete(PhiParam)),
    Theta(docsWords.docsNumber(), Discrete(ThetaParam)),
    docsWords(docsWords),
    stab_iteration(0),
    upd_iteration(0) {
}

int Algorithm::topicsNumber() const {
    return Phi.size();
}

bool Algorithm::update_needed() {
    ++upd_iteration;
    return (upd_iteration >= 100);
}

bool Algorithm::not_stabilised() {
    ++stab_iteration;
    std::cout << "  iteration made: " << stab_iteration << std::endl;
    return stab_iteration < 3;
}

void Algorithm::update_if_needed() {
    if (!update_needed()) {
        return;
    }
    for (size_t i = 0; i < Phi.size(); ++i) {
        Phi[i].update();
    }
    for (size_t i = 0; i < Theta.size(); ++i) {
        Theta[i].update();
    }
}

bool Algorithm::get_next_word(int & document, int & wordIdx) const {
    if (wordIdx + 1 < docsWords.wordsNumber(document)) {
        ++wordIdx;
    } else {
        wordIdx = 0;
        ++document;
    }
    return (document < docsWords.docsNumber());
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
                Phi[topic].inc(docsWords.word(d, w), delta);
                Theta[d].inc(topic, delta);
            }
            update_if_needed();
        }
    } while (not_stabilised());
    std::cout << "Algorithm PERFORMED. Total time is " << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}
