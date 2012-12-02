#include "file.h"
#include "tdw.h"
#include "vector_operations.h" // WHY NOT "vector_operations.h" ??

bool performPLSA(TopicsDocumentsWords & TDW) {
	cout.setf (std::ios::fixed, std::ios::floatfield);
    cout.precision (3);
	//E - step
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			vector<double> allHiddenParameteres(TDW.TopicNumber);
			for (size_t t = 0; t < TDW.TopicNumber; ++t) {
				allHiddenParameteres[t] = TDW.TopicWeights[t]*TDW.DocTopics[d][t]*TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][t];
			}
			TDW.DocsWords[d][w].HiddenParameters = SampleTopicVector(allHiddenParameteres, TDW.NumberOfHiddenParameters);
		}
	}

	cout << "DocsWords.HiddenParameters" << endl;
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		cout <<  "d" << d << " : ";
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			cout << TDW.DocsWords[d][w].WordNumber << "(";
			for (int t = 0; t < TDW.NumberOfHiddenParameters; ++t) {
				cout << TDW.DocsWords[d][w].HiddenParameters[t].Prob << " ";
			}
			cout << ") ";
		}
		cout << endl;
	}
	//M - Step
	vector<double> Stopics(TDW.TopicNumber, 0);
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		for (size_t w = 0; w < TDW.DocsWords[d].size(); ++w) {
			for (int t = 0; t < TDW.NumberOfHiddenParameters; ++t) {
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
			for (int t = 0; t < TDW.NumberOfHiddenParameters; ++t) {
				TDW.DocTopics[d][TDW.DocsWords[d][w].HiddenParameters[t].TopicNumber] += TDW.DocsWords[d][w].HiddenParameters[t].Prob * TDW.DocsWords[d][w].WordCount;
				TDW.WordTopics[TDW.DocsWords[d][w].WordNumber][TDW.DocsWords[d][w].HiddenParameters[t].TopicNumber] += TDW.DocsWords[d][w].HiddenParameters[t].Prob * TDW.DocsWords[d][w].WordCount;
			}
		}
	}
	TDW.MultDocTopics(1/S);
	TDW.MultWordTopics(1/S);

	cout << "S = " << S << endl;

	cout << "DocTopics" << endl;
	for (size_t d = 0; d < TDW.DocNumber(); ++d) {
		cout << "d" << d << ": " << TDW.DocTopics[d] << endl;
	}

	cout << "WordTopics" << endl;
	for (size_t w = 0; w < TDW.WordNumber; ++w) {
		cout << "w" << w << ": " << TDW.WordTopics[w] << endl;
	}

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
