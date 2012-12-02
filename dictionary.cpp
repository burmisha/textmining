#include "dictionary.h"
#include <time.h>
#include <fstream>
#include <iostream>

bool good_word(const std::string & word) {
    return ((word.length() > 3) && (word.length() < 30));
}

void Dictionary::add_docs(FileHandler file_handler) {
    file_handler.drop();
    clock_t zero_time = clock();
    while (file_handler.goto_next()) {
        clock_t first_time = clock();
        std::ifstream file(file_handler.filename().c_str()) ;
        while (!file.eof()) {
            std::string word;
            file >> word;
            if (good_word(word)) {
                add(word);
            }
        }
        std::cout << "\r  dictionary for text \"" << file_handler.short_filename()
                  << "\" built in " << (float)(clock() - first_time) / CLOCKS_PER_SEC;
        file.close();
    }
    std::cout << std::endl << "DICTIONARY: total time is " << (float)(clock() - zero_time) / CLOCKS_PER_SEC << std::endl;
}
