#include <stdlib.h>

#include "lda.h"

LDA::LDA(const DocsWords & docsWords,
         int topic_number,
         double phi_zero,   const std::vector<double> & phi_param,
         double theta_zero, const std::vector<double> & theta_param,
         const Distance & distance):
                Algorithm(docsWords, 
                          topic_number, 
                          phi_zero, phi_param, 
                          theta_zero, theta_param, 
                          distance) {
                    
    for (int d = 0; d < docsWords.docs_number(); ++d) {
        std::vector<std::vector<Pair> > document;
        for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
            document.push_back(std::vector<Pair>(docsWords.word_counter(d, w), Pair(0,0)));  // (-1,0)??
        }
        HiddenNumbers.push_back(document);
    }
}

void LDA::prepareDocWord(int document, int wordIdx) {
    // nothing to do
}

void LDA::prepareTopic(int document, int wordIdx, int index) {
    Pair topic = HiddenNumbers[document][wordIdx][index];
    double delta = (static_cast<int>(topic.counter) > 0) ? -1 : 0;
    Phi[topic.id].inc(docsWords.word_id(document, wordIdx), delta);
    updated_phi.insert(topic.id);
    Theta[document].inc(topic.id, delta);
    updated_theta.insert(document);

    std::vector<double> sampled_cumm_vector;
    double sum = 0;
    for (int t = 0; t < topic_number(); ++t) {
        sum += Phi[t](docsWords.word_id(document, wordIdx)) * Theta[document](t);
        sampled_cumm_vector.push_back(sum);
    }
    double goal = (double) rand() / RAND_MAX * sum;     // scaling
    int left = 0, right = topic_number() - 1, center;   // binary search
    while (right > left) {
        center = (left + right) / 2;
        if (sampled_cumm_vector[center] >= goal) {
            right = center;
        } else {
            left = center + 1;
        }
    }
    HiddenNumbers[document][wordIdx][index].id = left;
}

double LDA::getDelta(int document, int wordIdx, int i) const {
    return 1; // yes, that's all
}

