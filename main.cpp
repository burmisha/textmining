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
#include "quality.h"

int main() {
    const int topic_number = 5;
    setlocale(LC_ALL, "");
    std::ofstream answer_file;
    answer_file.open("Jun11-2-LDA.txt");

    FileHandler learning_files(std::string("d:\\base\\AlexRom\\final\\text "),
                               std::string(".txt"), 1 , 2000);

    FileHandler control_files(std::string("d:\\base\\AlexRom\\final\\text "),
                              std::string(".txt"), 2001 , 2200);

    Map dictionary;
    dictionary.add_docs(learning_files);
    DocsWords docs_words(dictionary, learning_files);
    std::pair<DocsWords, DocsWords> control(docs_words.build_control(dictionary , control_files));
    DocsWords train = control.first, test = control.second;


    std::vector<double> alpha(docs_words.unique_words_number(), 0.1);
    std::vector<double> beta(topic_number, 0.1);

    PhiTheta phi_theta(docs_words, KullbackLeibler(), alpha, beta);
    hidden_type hidden = LDA_hidden(docs_words);
    for (int i = 0; i < 100; ++i) {
        double time;
        {
            //hidden_type hidden = PLSA_hidden(docs_words, topic_number);   PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(1000);          stabilized_after stabilized(1);
            time = perform_algorithm(docs_words, phi_theta, hidden, prepare, get_delta, update, stabilized);
        }

        PhiTheta train_phi_theta(phi_theta.save_phi(train));
        train_phi_theta.set_phi_fixed(true);
        {
            //hidden_type hidden_train = PLSA_hidden(train, topic_number);      PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            hidden_type hidden_train = LDA_hidden(train);                     LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(100);           stabilized_after stabilized(5);
            perform_algorithm(train, train_phi_theta, hidden_train, prepare, get_delta, update, stabilized);
        }
        std::cout << "i = " << i << std::endl;
        answer_file << time << " "
                    << count_quality(test, train_phi_theta, KullbackLeibler()) << " "
                    << count_quality(test, train_phi_theta, Chi_squared()) << " "
                    << count_quality(test, train_phi_theta, Hellinger()) << std::endl;
    }
    std::cout << std::endl;
//    double qualityKL = count_quality(test, train_phi_theta, KullbackLeibler());
//    answer_file << "qualityKL = " << qualityKL << std::endl;
//    double qualityChi = count_quality(test, train_phi_theta, Chi_squared());
//    answer_file << "qualityChi = " << qualityChi << std::endl;
//    double qualityH = count_quality(test, train_phi_theta, Hellinger());
//    answer_file << "qualityH = " << qualityH << std::endl;

    /*phi_theta.print(answer_file, dictionary);
    answer_file << std::endl << "Algorithm PERFORMED. Total time is " << time << std::endl;*/


    answer_file.close();
    // system("pause");
}
