#include "tdw.h"
#include "file.h"
#include "vector_operations.h" // WHY NOT "vector_operations.h" ??

vector<HiddenParameter> SampleTopicVector(const vector<double> & InitialVector, int SampleSize) {
    vector<HiddenParameter> ResultVector(SampleSize);
    vector<bool> UsedElements(InitialVector.size(), false);
    int SampledComponents = 0;
    do {
        double randValue = (double)rand() / RAND_MAX;
        SummedVector(InitialVector);
        size_t candidate = BinaryMinGreaterSearch(NormalizedVector(SummedVector(InitialVector)), randValue);
        if (!UsedElements[candidate]) {
            UsedElements[candidate] = true;
            ResultVector[SampledComponents] = HiddenParameter(candidate, InitialVector[candidate]);
            SampledComponents += 1;
        }
    } while (SampledComponents < SampleSize);
    return ResultVector;
};

TopicsDocumentsWords::TopicsDocumentsWords(const Dictionary & Dict, int TopicNumber, int NumberOfHiddenParameters):
    TopicNumber(TopicNumber),
    WordNumber(Dict.WordNumber()),
    NumberOfHiddenParameters(NumberOfHiddenParameters) {
    fileHandler files;
    while(files.areMoreFiles()) {
        clock_t firstTime = clock();
        ifstream file(files.NewFileName().c_str());

        vector<int> Doc;
        while(!file.eof()) {
            string word;
            file >> word;
            if(Dict.getNumber(word) >= 0) {
                Doc.push_back(Dict.getNumber(word));
            }
        }
        cout << Doc;
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
        cout << "Init for text #" << files.CurrentFileID() << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
        file.close();
    }
    cout << "WordNumber: " << WordNumber << endl;
    DocTopics.resize(DocNumber(), vector<double>(TopicNumber, 1));
    WordTopics.resize(WordNumber, vector<double>(TopicNumber, 1));
    TopicWeights.resize(TopicNumber, 1);
    DocWeights.resize(DocNumber(), 1);
}
