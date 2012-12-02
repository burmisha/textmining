#include <string>
#include <time.h>
#include <iostream>

#include "map.h"
#include "docs_words.h"
#include "plsa.h"
#include "lda.h"
#include "distance.h"

int main() {
    const int TopicNumber = 5;
    setlocale(LC_ALL, "");
    FileHandler file_handler(std::string("d:\\base\\AlexRom\\final\\text "),
                             std::string(".txt"), 1 , 20);
    Map dictionary;
    dictionary.add_docs(file_handler);
    DocsWords docsWords(dictionary, file_handler);
    PLSA count(docsWords, TopicNumber, KullbackLeibler());
    //sLDA count(docsWords, TopicNumber, 0.4, std::vector<double>(dictionary.wordNumber() , 0.4), 0.6, std::vector<double>(TopicNumber, 0.6), KL);
    count.perform();

    //std::cout << count;

    std::ofstream answer_file;
    answer_file.open("Jun08_19-36.txt");
    count.print(answer_file, dictionary);
    answer_file.close();
    // system("pause");
}
