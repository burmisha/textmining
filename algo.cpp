#include "header.h"

struct HiddenParameter {
	int TopicNumber;
	double Prob;
	HiddenParameter(int TopicNumber, double Prob) : TopicNumber(TopicNumber), Prob(Prob) {};
	HiddenParameter(): TopicNumber(-1), Prob(0) {};
};

struct DocWordElement {
	int WordNumber;
	int WordCount;
	vector<HiddenParameter> HiddenParameters;
	int Topic;
	DocWordElement(	int WordNumber,	int WordCount):
        WordNumber(WordNumber), WordCount(WordCount), Topic(-1) { //IMPOSSIBLE
    }
};

const int NumberOfHiddenParameters = 20;
class TopicsDocumentsWords {
public:
	size_t TopicNumber;
	size_t WordNumber;
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
	TopicsDocumentsWords(const Dictionary & , int );
};
TopicsDocumentsWords::TopicsDocumentsWords(const Dictionary & Dict, int TopicNumber):
		TopicNumber(TopicNumber),
        WordNumber(Dict.WordNumber())
		{
        const string prefix("d:\\base_main\\AlexRom\\final\\text ");
        const string extension(".txt");
        for(int i = 1 ; i <= 7146 ; ++i) {  //7146
            clock_t firstTime = clock();
            std::stringstream number;
            number << i;
            ifstream file((prefix + number.str() + extension).c_str()) ;
            vector<int> Doc;
            while (!file.eof()) {
                string word;
                file >> word;
                if (Dict.getNumber(word) > 0) {
                    Doc.push_back(Dict.getNumber(word));
                }
            }
            sort(Doc.begin(), Doc.end());
            vector<DocWordElement> DocWordElements;
            size_t i = 0, s = 0;
            while (i+1 < Doc.size()) {
                if (Doc[i] != Doc[i+1]) {
                    DocWordElements.push_back(DocWordElement(Doc[i],i-s+1));
                    s = i+1;
                }
                ++i;
            }
            DocWordElements.push_back(DocWordElement(Doc[i],i-s+1));
            DocsWords.push_back(DocWordElements);
            cout << "Время обработки текста #" << i << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
            file.close();
        }
}

vector<HiddenParameter> SampleTopicVector(const vector<double> & InitialVector, int SampleSize) {
	vector<HiddenParameter> ResultVector(SampleSize);
	vector<bool> UsedElements(InitialVector.size(), false);
	vector<double> Summed(InitialVector.size(), 0);
	Summed[0] = InitialVector[0];
	for (size_t i = 0; i+1 < Summed.size(); ++i) {
		Summed[i+1] = Summed[i] + InitialVector[i+1];
	}
	int SampledComponents = 0;
	do {
		double randValue = (double)rand()/RAND_MAX*Summed[Summed.size()-1];
		int left = 0, right = InitialVector.size()-1; //binarySearch
		int center;
		while (right-left > 1) {
			center = (left+right)/2;
			if (Summed[center] > randValue) {
				right = center;
			} else {
				left = center;
			}
		}
		if (!UsedElements[left]) {
			UsedElements[left] = true;
			ResultVector[SampledComponents] = HiddenParameter(left, InitialVector[left]);
			++SampledComponents;
		}
	} while (SampledComponents < SampleSize);
	return ResultVector;
};

template <typename T>
ostream & operator << (ostream& out, const vector<T> & vec) {
	for (size_t i = 0; i < vec.size() ; ++i) {
			out << vec[i] << " ";
	}
	return out;
}

bool performPLSA(TopicsDocumentsWords & TDW) {
	//E - step
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			vector<double> allHiddenParameteres(TDW.TopicNumber);
			for (size_t t = 0; t < TDW.TopicNumber; ++t) {
				allHiddenParameteres[t] = TDW.TopicWeights[t]*TDW.DocTopics[d][t]*TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][t];
			}
			TDW.DocsWords[d][w].HiddenParameters = SampleTopicVector(allHiddenParameteres, NumberOfHiddenParameters);
		}
	}
	//M - Step
	vector<double> Stopics(TDW.TopicNumber, 0);
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			for (int t = 0; t < NumberOfHiddenParameters; ++t) {
				Stopics[TDW.DocsWords[d][w].HiddenParameters[t].TopicNumber] += TDW.DocsWords[d][w].HiddenParameters[t].Prob * TDW.DocsWords[d][w].WordCount;
			}
		}
	}
	double S = 0;
	for (size_t t = 0; t < Stopics.size(); ++t) {
		S += Stopics[t];
	}
	for (size_t t = 0; t < TDW.TopicNumber; ++t) {
		TDW.TopicWeights[t] = Stopics[t]/S;
	}; 	// p_{t} is formed


	TDW.MultDocTopics(0);	//	p(d|t) := 0
	TDW.MultWordTopics(0);	//	p(w|t) := 0
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			for (int t = 0; t < NumberOfHiddenParameters; ++t) {
				TDW.DocTopics[d][TDW.DocsWords[d][w].HiddenParameters[t].TopicNumber] += TDW.DocsWords[d][w].HiddenParameters[t].Prob * TDW.DocsWords[d][w].WordCount;
				TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][TDW.DocsWords[d][w].HiddenParameters[t].TopicNumber] += TDW.DocsWords[d][w].HiddenParameters[t].Prob * TDW.DocsWords[d][w].WordCount;
			}
		}
	}
	TDW.MultDocTopics(1/S);
	TDW.MultWordTopics(1/S);

	return true;
};

bool performLDA(TopicsDocumentsWords & TDW) {
	double alpha = 0.1;
	double beta = 0.2;
	int TotalSampleNumber = 10;

	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t t = 0; t < TDW.TopicNumber; ++t) {
			TDW.DocTopics[d][t] = alpha;
		}
	}
	for (size_t t = 0; t < TDW.TopicNumber; ++t) {
		for (size_t w = 0; w < TDW.WordNumber; ++w) {
			TDW.WordTopics[w][t] = beta;
		}
	}
	for (size_t t = 0; t < TDW.TopicNumber; ++t) {
		TDW.TopicWeights[t] = beta * TDW.WordNumber;
	}
	for (size_t t = 0; t < TDW.DocNumber(); ++t) {
		TDW.DocWeights[t] = alpha * TDW.TopicNumber;
	}

	for (int SampleNumber = 0; SampleNumber < TotalSampleNumber; ++SampleNumber) {
		for (size_t d = 0; d < TDW.DocNumber(); ++d) {
			for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
				vector<double> allHiddenParameteres(TDW.TopicNumber);
				for (size_t t = 0; t < TDW.TopicNumber; ++t) {
					allHiddenParameteres[t] = TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][t]
												/ TDW.TopicWeights[t]
												* TDW.DocTopics[d][t]
												/ TDW.DocWeights[d];
				}
				vector<HiddenParameter> HP = SampleTopicVector(allHiddenParameteres, 1);
				int topic ;
				if (SampleNumber >= 1) {
					topic = TDW.DocsWords[d][w].Topic;
					--TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][topic];
					--TDW.DocTopics[d][topic];
					--TDW.DocWeights[d];
					--TDW.TopicWeights[topic];
				}
				topic = HP[1].TopicNumber;
				TDW.DocsWords[d][w].Topic = topic;
				++TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][topic];
				++TDW.DocTopics[d][topic];
				++TDW.DocWeights[d];
				++TDW.TopicWeights[topic];
			}
		}
	}
	return true;
}
