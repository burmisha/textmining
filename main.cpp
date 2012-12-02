#include "header.h"
#include "tree.h"
#include "tdw.h"
#include "algo.h"
int main() {
    setlocale(LC_ALL, "");
    clock_t firstTime = clock();
    tree TreeDict;
    BuildDictionary(TreeDict);
    cout << "Total time :" << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
    TopicsDocumentsWords TDW(TreeDict, 2);
	for (int i = 0; i < 5; ++i) {
		cout << i << endl;
		performPLSA(TDW);
	}
    system("pause");
}
