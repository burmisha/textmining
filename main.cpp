//#include <string>
//#include <time.h>
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
#include "quality.h"

int main() {
    const int topic_number = 20;
    setlocale(LC_ALL, "");
    std::ofstream answer_file;
    answer_file.open("PLSA_t1000c200_t20_u20_s5_i100.txt");
    answer_file << std::unitbuf;
    FileHandler learning_files(std::string("d:\\base\\AlexRom\\final\\text "), std::string(".txt"), 1 ,     1000);
    FileHandler  control_files(std::string("d:\\base\\AlexRom\\final\\text "), std::string(".txt"), 1001 ,  1200);
    Map dictionary;
    dictionary.add_docs(learning_files);
    DocsWords docs_words(dictionary, learning_files);
    std::pair<DocsWords, DocsWords> control(docs_words.build_control(dictionary , control_files));
    DocsWords train = control.first, test = control.second;

    std::vector<double> alpha(docs_words.unique_words_number(), 0.1);
    std::vector<double> beta(topic_number, 0.1);
    PhiTheta phi_theta(docs_words, KullbackLeibler(), alpha, beta);

    //hidden_type hidden = PLSA_hidden(docs_words, topic_number);
    hidden_type hidden = LDA_hidden(docs_words);

    for (int i = 0; i < 10; ++i) {
        double time;
        {
            //PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(100);          stabilized_after stabilized(1);
            time = perform_algorithm(docs_words, phi_theta, hidden, prepare, get_delta, update, stabilized);
        }

        PhiTheta train_phi_theta(phi_theta.save_phi(train));
        train_phi_theta.set_phi_fixed(true);
        {
            //hidden_type hidden_train = PLSA_hidden(train, topic_number);      PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            hidden_type hidden_train = LDA_hidden(train);                     LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(20);           stabilized_after stabilized(5);
            perform_algorithm(train, train_phi_theta, hidden_train, prepare, get_delta, update, stabilized);
        }
        std::cout << "i = " << i << std::endl;
        answer_file << time << " "
                    << count_quality(test, train_phi_theta, KullbackLeibler()) << " "
                    << count_quality(test, train_phi_theta, Chi_squared()) << " "
                    << count_quality(test, train_phi_theta, Hellinger()) << std::endl;
    }
    std::cout << std::endl;
    answer_file.close();
}
