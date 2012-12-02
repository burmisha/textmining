#include "file.h"
int checkWord(const string & word) {
    if((word.length() < 3) || (word.length() > 32)) {
        return 0;
    }
    return 1;
}

void BuildDictionary(Dictionary & Dict) {
    fileHandler files;
    while(files.areMoreFiles()) {
        clock_t firstTime = clock();
        ifstream file(files.NewFileName().c_str()) ;
        while(!file.eof()) {
            string word;
            file >> word;
            Dict.add(word);
            //Dict.print();
        }
        cout << "Time for text #" << files.CurrentFileID() << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
        file.close();
    }
}
