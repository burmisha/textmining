#include <string>
#include <time.h>
#include <iostream>

#include "tree.h"
#include "docs_words.h"
#include "plsa.h"
#include "lda.h"
#include "distance.h"

int main() {
    const int TopicNumber = 5;
    setlocale(LC_ALL, "");
    std::cout.setf (std::ios::fixed, std::ios::floatfield);
    std::cout.precision (6);

    FileHandler file_handler(std::string("d:\\base\\AlexRom\\final\\text "),
                             std::string(".txt"), 1 , 200);
    Tree dictionary;
    dictionary.build(file_handler);
    DocsWords docsWords(dictionary, file_handler);
    KullbackLeibler KL;
    PLSA count(docsWords, TopicNumber, KL);
	//sLDA count(docsWords, TopicNumber, 0.4, std::vector<double>(dictionary.wordNumber() , 0.4), 0.6, std::vector<double>(TopicNumber, 0.6), KL);
    count.perform();

    //std::cout << count;
    count.print(std::cout, dictionary);
    // system("pause");
}
