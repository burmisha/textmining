#include "plsa.h"
#include <vector>
#include <stdlib.h>

#include <iostream>

PLSA::PLSA(const DocsWords & docsWords,
           int topic_number,
           const Distance & distance):
    Algorithm(docsWords,
              topic_number,
              0, std::vector<double>(docsWords.unique_words_number(), 0),
              0, std::vector<double>(topic_number, 0),
              distance) {

    std::vector<Pair> HidNum;
    for (int i = 0; i < static_cast<int>(topic_number); ++i) {
        HidNum.push_back(Pair(i, 0));
    }
    for (int d = 0; d < docsWords.docs_number(); ++d) {
        std::vector<std::vector<Pair> > document(docsWords.unique_words_number(d), HidNum);
        HiddenNumbers.push_back(document);
    }
}

void PLSA::prepareDocWord(int document, int wordIdx) {
    Z = 0;
    const std::vector<Pair> & HidNum = HiddenNumbers[document][wordIdx];
    for (size_t i = 0; i < HidNum.size(); ++i) {
        int topic = HidNum[i].id;
        int word = docsWords.word_id(document, wordIdx);

        Z += Phi[topic](word) * Theta[document](topic);
        //std::cout << Phi[topic](word)  <<  " * "  << Theta[document](topic) << " =+ " << Z << std::endl;
        /*std::cout << " Z +=" <<  Phi[topic](word) << " * " << Theta[document](topic) << std::endl;
        if (Z != Z) {
            std::cout << " *** ERROR: nan. d" << document << ", word" << wordIdx << " of"  << docsWords.unique_words_number(document) << " i=" << i << std::endl<< *this;
            exit(-10);
        }*/
    }
    //std::cout << " Z =" << Z << std::endl;

}

void PLSA::prepareTopic(int document, int wordIdx, int topic) {
    // nothing to do
}

double PLSA::getDelta(int document, int wordIdx, int i) const {
    const Pair & pair = HiddenNumbers[document][wordIdx][i];
    if ( (pair.counter > 0)
         || (  (Phi[pair.id](docsWords.word_id(document, wordIdx) > 0)
            && (Theta[document](pair.id) > 0)) ) ) {
        /*std::cout << "Z : " << Z << "delta: " << docsWords.word_counter(document, wordIdx)
                                  * Phi[pair.id](docsWords.word_id(document, wordIdx))
                                  * Theta[document](pair.id) / Z - pair.counter;*/
        double result = docsWords.word_counter(document, wordIdx)
                * Phi[pair.id](docsWords.word_id(document, wordIdx))
                * Theta[document](pair.id) / Z - pair.counter;
        /*if (result != result) {
            std::cout   << " *** ERROR: getDelta. a" << docsWords.word_counter(document, wordIdx)
                        << ", b" <<Phi[pair.id](docsWords.word_id(document, wordIdx))
                        << ", c"  << Theta[document](pair.id)
                        << ", z" << Z
                        << ", h" << pair.counter
                        << std::endl<< *this;
            exit(-10);
        }*/
        return result;
        }
    else {
        return 0;
    }
}
