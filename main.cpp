#include "header.h"

int main() {
    setlocale(LC_ALL, "");
    clock_t firstTime = clock();
    tree TreeDict;
    BuildDictionary(TreeDict);
    cout << "Общее время работы = " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
    system("pause");
}
