#include "dictionary.h"
#include <time.h>
#include <fstream>
#include <iostream>
//#include <sstream>

int checkWord(const std::string & word) {
    if ((word.length() < 3) || (word.length() > 32)) {
        return 0;
    }
    return 1;
}

void Dictionary::build(FileHandler fileHandler) {
    fileHandler.drop();
    clock_t firstTime = clock();
    while (fileHandler.goto_next()) {
        clock_t firstTime = clock();
        std::ifstream file(fileHandler.filename().c_str()) ;
        while (!file.eof()) {
            std::string word;
            file >> word;
            add(word);
        }
        std::cout << "  dictionary for text \"" << fileHandler.short_filename() << "\" built in " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << std::endl;
        file.close();
    }
    std::cout << "DICTIONARY: total time is " << (float)(clock() - firstTime) / CLOCKS_PER_SEC << std::endl;
}
