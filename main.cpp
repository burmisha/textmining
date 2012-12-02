#include "header.h"
#include "tree.h"
#include "tdw.h"
#include "algo.h"
const int TopicNumber = 3;
const int SampleSize = 2;
int main() {
    setlocale(LC_ALL, "");
    clock_t firstTime = clock();
    tree TreeDict;
    BuildDictionary(TreeDict);
    cout << "Total time :" << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
    TopicsDocumentsWords TDW(TreeDict, TopicNumber, SampleSize);
    for (int i = 0; i < 2; ++i) {
        performPLSA(TDW);
    }
    system("pause");
}
