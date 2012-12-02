#include <iostream>
#include <sstream>

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
    const double gamma = 0.001;
    const std::string prefix("d:\\base\\Aleksander_texts_final\\text ");
    const std::string suffix(".txt");
    setlocale(LC_ALL, "");
    std::ofstream answer_file;

    const int begin_doc = 1;
    const int middle_doc = 100;
    const int last_doc = 120;
    const int update_freq = 5000;
    const int stab_freq_on_training = 20;
    const int total_iterations = 1000;

    std::stringstream answer_filename;
    answer_filename << "PLSA_ROBx1_t" << middle_doc - begin_doc + 1 << "c" << last_doc - middle_doc << "_t" << topic_number << "_u";
    answer_filename << update_freq;
    answer_filename << "_s" << stab_freq_on_training << "_i" << total_iterations;
    answer_filename << "_gam" << (int)(gamma * 10000);
    //std::cout << gamma << " " << (int)(gamma * 100) << std::endl;

    answer_filename << ".txt";
    answer_file.open(answer_filename.str().c_str());
    //answer_file.open("LDA_t1000c200_t20_a02b02_h6_U5000_s20_i500.txt");
    //answer_file.open("PLSA_t1000c200_t20_Ud_s5_i2000.txt");
    answer_file << std::unitbuf;

    FileHandler learning_files(prefix, suffix, begin_doc , middle_doc);
    //FileHandler control_files(prefix, suffix, begin_doc , middle_doc/5); // !!!!!!!!!!!!!!!!
    FileHandler  control_files(prefix, suffix, middle_doc + 1 , last_doc);
    Map dictionary;
    dictionary.add_docs(learning_files);
    DocsWords docs_words(dictionary, learning_files);
    std::pair<DocsWords, DocsWords> control(docs_words.build_control(dictionary , control_files));
    DocsWords train = control.first, test = control.second;

    const double param = 1; // 1 - for PLSA.
    std::vector<double> alpha(docs_words.unique_words_number(), param);
    std::vector<double> beta(topic_number, param);
    PhiTheta phi_theta(docs_words, KullbackLeibler(), alpha, beta);

    //Hidden_Collection hidden = PLSA_hidden(docs_words, topic_number);
    //Hidden_Collection hidden = LDA_hidden_limited(docs_words, 6);
    Hidden_Collection hidden = PLSA_robust(docs_words, topic_number, gamma);

    for (int i = 0; i < total_iterations; ++i) {
        double time;
        {
            PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            //LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(update_freq);
            //update_after_document update(0);
            //update_after_collection update(0);
            stabilized_after stabilized(1);
            time = perform_algorithm(docs_words, phi_theta, hidden, prepare, get_delta, update, stabilized, gamma);
        }

        PhiTheta train_phi_theta(phi_theta.save_phi(train));
        train_phi_theta.set_phi_fixed(true);
        {
            //Hidden_Collection hidden_train = PLSA_hidden(train, topic_number);
            Hidden_Collection hidden_train = PLSA_robust(train, topic_number, gamma);
            PLSA_prepare prepare;   PLSA_GetDelta get_delta;
            //Hidden_Collection hidden_train = LDA_hidden_limited(train, 6);                     LDA_prepare prepare;    LDA_GetDelta get_delta;
            update_every update(update_freq);
            //update_after_document update(0);
            //update_after_collection update(0);
            stabilized_after stabilized(stab_freq_on_training);
            perform_algorithm(train, train_phi_theta, hidden_train, prepare, get_delta, update, stabilized, gamma); // LOOK: gamma is 0
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
