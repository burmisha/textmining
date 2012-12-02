#include <string>
#include <time.h>
#include <iostream>

#include "tree.h"
#include "docs_words.h"
#include "plsa.h"
#include "lda.h"

int main() {
    const int TopicNumber = 2;
    setlocale(LC_ALL, "");

    FileHandler file_handler(std::string("d:\\base\\AlexRom\\final\\text "),
                             std::string(".txt"), 1 ,3);
    Tree dictionary;
    dictionary.build(file_handler);
    DocsWords docsWords(dictionary, file_handler);
    // PLSA count(docsWords, TopicNumber);
	LDA count(docsWords, TopicNumber, std::vector<double>(dictionary.wordNumber() + 1, 0.4), std::vector<double>(TopicNumber + 1, 0.6));
    count.perform();
    // system("pause");
}
