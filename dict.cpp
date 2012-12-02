#include "header.h"
int checkWord(const string & word) {
    if ((word.length() < 3) || (word.length() > 32)) {
        return 0;
    }
    return 1;
}

void BuildDictionary(Dictionary & Dict) {
    const string prefix("d:\\base_main\\AlexRom\\final\\text ");
    const string extension(".txt");
    for(int i = 1 ; i <= 7146 ; ++i) {  //7146
        clock_t firstTime = clock();
        std::stringstream number;
        number << i;
        ifstream file((prefix + number.str() + extension).c_str()) ;
        while(!file.eof()) {
            string word;
            file >> word;
            Dict.add(word);
            //Dict.print();
        }
        cout << "Время обработки текста #" << i << " : " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << endl;
        file.close();
    }
}
