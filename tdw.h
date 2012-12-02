#ifndef TDW_H
#define TDW_H
#include "header.h"
#include "file.h"

struct HiddenParameter {
	int TopicNumber;
	double Prob;
	HiddenParameter(int TopicNumber, double Prob) : TopicNumber(TopicNumber), Prob(Prob) {};
	HiddenParameter(): TopicNumber(-1), Prob(0) {};
};

vector<HiddenParameter> SampleTopicVector(const vector<double> &, int );

struct DocWordElement {
	int WordNumber;
	int WordCount;
	vector<HiddenParameter> HiddenParameters;
	int Topic;
	DocWordElement(	int WordNumber,	int WordCount):
        WordNumber(WordNumber), WordCount(WordCount), Topic(-1) { //IMPOSSIBLE
    }
};

class TopicsDocumentsWords {
    public:
        size_t TopicNumber;
        size_t WordNumber;
		int NumberOfHiddenParameters;
        vector<vector<double> > DocTopics;			// p(d|t) = n_{td}
        vector<vector<double> > WordTopics;			// p(w|t) = n_{wt}
        vector<double> TopicWeights;				// p(t)
        vector<double> DocWeights;					// n(d)
        vector<vector<DocWordElement > > DocsWords;
        size_t DocNumber() {
            return DocsWords.size();
        }
        void MultDocTopics(double multiplier) {
            for (size_t d = 0; d < DocNumber(); ++d) {
                for (size_t t = 0; t < TopicNumber; ++t) {
                    DocTopics[d][t] *= multiplier;
                }
            }
        }
        void MultWordTopics(double multiplier) {
            for (size_t t = 0; t < TopicNumber; ++t) {
                for (size_t w = 0; w < WordNumber; ++w) {
                    WordTopics[w][t] *= multiplier;
                }
            }
        }
        TopicsDocumentsWords(const Dictionary & , int, int  );
};

#endif
