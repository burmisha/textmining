#include <string>
#include <time.h>
#include <iostream>

#include "file_handler.h"
#include "dict_map.h"
#include "docs_words.h"
#include "distance.h"
#include "phi_theta.h"
#include "hidden.h"
#include "algorithm.h"
#include "prepare.h"
#include "get_delta.h"
#include "update_time.h"
#include "stabilized.h"

int main() {
    const int topic_number = 5;
    setlocale(LC_ALL, "");
    FileHandler file_handler(std::string("d:\\base\\AlexRom\\final\\text "),
                             std::string(".txt"), 1 , 100);

    std::ofstream answer_file;
    answer_file.open("Jun09.txt");

    Map dictionary;
    dictionary.add_docs(file_handler);
    DocsWords docs_words(dictionary, file_handler);

    std::vector<double> alpha(docs_words.unique_words_number(), 0.1);
    std::vector<double> beta(topic_number, 0.1);

    PhiTheta phi_theta(docs_words, KullbackLeibler(), alpha, beta);
    phi_theta.print(answer_file, dictionary);
    hidden_type hidden = PLSA_hidden(docs_words, topic_number);
    PLSA_prepare prepare;
    PLSA_GetDelta get_delta;
    update_every update(100);
    stabilized_after stabilized(10);
    Algorithm algorithm(docs_words, phi_theta, hidden,
                        prepare, get_delta, update, stabilized);
    //algorithm();



    phi_theta.print(answer_file, dictionary);
    answer_file.close();
    // system("pause");
}
