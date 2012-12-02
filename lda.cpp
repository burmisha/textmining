#include <stdlib.h>

#include "algorithm.h"
#include "lda.h"

void LDA::prepareDocWord(int document, int wordIdx) {
    // nothing to do
}

void LDA::prepareTopic(int document, int wordIdx, int index) {
    Pair topic = HiddenNumbers[document][wordIdx][index];
    double delta = -(static_cast<int>(topic.counter) > 0);
    Phi[topic.id].inc(docsWords.word(document, wordIdx), delta);
    Theta[document].inc(topic.id, delta);

    std::vector<double> sampled_cumm_vector;
    double sum = 0;
    for (int t = 0; t < topicsNumber(); ++t) {
        sum += Phi[t](docsWords.word(document, wordIdx)) * Theta[document](t);
        sampled_cumm_vector.push_back(sum);
    }
    double goal = (double) rand() / RAND_MAX * sum;             // scaling
    int left = 0, right = topicsNumber() - 1, center;  // binary search
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

LDA::LDA(const DocsWords & docsWords,
         int topic_number,
         const std::vector<double> & PhiParam,
         const std::vector<double> & ThetaParam):
                Algorithm(docsWords, topic_number, PhiParam, ThetaParam) {
    for (int d = 0; d < docsWords.docsNumber(); ++d) {
        std::vector<std::vector<Pair> > document;
        for (int w = 0; w < docsWords.wordsNumber(d); ++w) {
            document.push_back(std::vector<Pair>(docsWords.counter(d, w), Pair(0,0)));  // (-1,0)??
        }
        HiddenNumbers.push_back(document);
    }
}
