#include "tdw.h"
#include "file.h"

template <typename T>
vector<T> SummedVector(const vector<T> & InitialVector) {
	vector<T> Summed(InitialVector.size(), 0);
    Summed[0] = InitialVector[0];
    for (size_t i = 0; i + 1 < Summed.size(); ++i) {
        Summed[i+1] = Summed[i] + InitialVector[i+1];
    }
	return Summed;
};

//int BinaryMinGreaterSearch()

vector<HiddenParameter> SampleTopicVector(const vector<double> & InitialVector, int SampleSize) {
    vector<HiddenParameter> ResultVector(SampleSize);
    vector<bool> UsedElements(InitialVector.size(), false);
	vector<double> Summed = SummedVector(InitialVector);
    int SampledComponents = 0;
    do {
        double randValue = (double)rand() / RAND_MAX * Summed[Summed.size()-1];
        int left = 0, right = InitialVector.size() - 1; //binarySearch
        int center;
        while (right > left) {
            center = (left + right) / 2;
            if (Summed[center] >= randValue) {
                right = center;
            } else {
                left = center + 1;
            }
        }
        if (!UsedElements[left]) {
            UsedElements[left] = true;
            ResultVector[SampledComponents] = HiddenParameter(left, InitialVector[left]);
            ++SampledComponents;
        }
    } while(SampledComponents < SampleSize);
    return ResultVector;
};

TopicsDocumentsWords::TopicsDocumentsWords(const Dictionary & Dict, int TopicNumber):
    TopicNumber(TopicNumber),
    WordNumber(Dict.WordNumber()) {
    fileHandler files;
    while(files.areMoreFiles()) {
        clock_t firstTime = clock();
        ifstream file(files.NewFileName().c_str());

        vector<int> Doc;
        while(!file.eof()) {
            string word;
            file >> word;
            if(Dict.getNumber(word) > 0) {
                Doc.push_back(Dict.getNumber(word));
            }
        }

        sort(Doc.begin(), Doc.end());

        vector<DocWordElement> DocWordElements;
        size_t i = 0, s = 0;
        while(i + 1 < Doc.size()) {
            if(Doc[i] != Doc[i+1]) {
                DocWordElements.push_back(DocWordElement(Doc[i], i - s + 1));
                s = i + 1;
            }
            ++i;
        }

        DocWordElements.push_back(DocWordElement(Doc[i], i - s + 1));
        DocsWords.push_back(DocWordElements);
        cout << "Time for text #" << files.CurrentFileID() << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
        file.close();
    }

    DocTopics.resize(DocNumber(), vector<double>(TopicNumber, 1));
    WordTopics.resize(WordNumber, vector<double>(TopicNumber, 1));
    TopicWeights.resize(TopicNumber, 1);
    DocWeights.resize(DocNumber(), 1);
}
