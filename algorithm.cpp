#include <time.h>
#include <stdlib.h>

#include "algorithm.h"

Algorithm::Algorithm(const DocsWords & docsWords,
                     int topic_number,
                     double phi_zero,   const std::vector<double> & phi_param,
                     double theta_zero, const std::vector<double> & theta_param,
                     const Distance & distance):
    docsWords(docsWords),
    Phi(topic_number, Discrete(docsWords.unique_words_number())),
    Theta(docsWords.docs_number(), Discrete(topic_number)),
    Phi_zero(phi_zero), PhiParam(phi_param),
    Theta_zero(theta_zero), ThetaParam(theta_param),
    stab_iteration(0),
    upd_iteration(0) {
        std::vector<double> words_counter(docsWords.unique_words_number(), 0);
        for (int t = 0; t < topic_number; ++t) {
            int d = rand() * docsWords.docs_number() / RAND_MAX;
            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                Phi[t].inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
                words_counter[docsWords.word_id(d, w)] += docsWords.word_counter(d, w);
            }
            Phi[t].update(0, std::vector<double>(Phi[t].size(), 0));
        }

        const int R = 3 * topic_number;
        for (int r = 0; r < R; ++r) {
            int d = rand() * docsWords.docs_number() / RAND_MAX;

            /* FIND BEST TOPIC */
            Discrete words_distribution(docsWords.unique_words_number()); // $$\hat p_d$$
            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                    words_distribution.inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
            }
            int best_topic = 0;
            double min_distance = distance(Phi[0], words_distribution);
            for (int t = 1; t < topic_number; ++t) {
                if (distance(Phi[t], words_distribution) < min_distance) {
                    best_topic = t;
                }
            }

            for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                Phi[best_topic].inc(docsWords.word_id(d, w), docsWords.word_counter(d, w));
                words_counter[docsWords.word_id(d, w)] += docsWords.word_counter(d, w);
            }
            Phi[best_topic].update(0, std::vector<double>(Phi[best_topic].size(), 0));
        }

        for (int d = 0; d < docsWords.docs_number(); ++d) {
            for (int t = 0; t < topic_number; ++t) {
                for (int w = 0; w < docsWords.unique_words_number(d); ++w) {
                    Theta[d].add_prob(t, static_cast<double>(docsWords.word_counter(d, w))
                                         * Phi[t].get_counter(docsWords.word_id(d,w))
                                         / docsWords.total_words_number(d)
                                         / words_counter[docsWords.word_id(d, w)]);
                }
            }
        }

        /* REGULARIZE */
        const double small = 0.05;
        const double alpha = small / topic_number;
        const std::vector<double> alphas(topic_number, alpha);
        for (int d = 0; d < docsWords.docs_number(); ++d) {
            Theta[d].update(small, alphas);
        }
        const double beta = small / docsWords.unique_words_number();
        const std::vector<double> betas(docsWords.unique_words_number(), beta);
        for (int t = 0; t < topic_number; ++t) {
            Phi[t].update(small, betas);
        }
}

int Algorithm::topic_number() const {
    return Phi.size();
}

bool Algorithm::update_needed() {
    ++upd_iteration;
    if (upd_iteration > 100) {
        upd_iteration = 0;
        //std::cout << "upd ";
        return true;
    } else {
        return false;
    }
}

bool Algorithm::not_stabilised() {
    ++stab_iteration;
    //std::cout << *this << "  iteration made: " << stab_iteration << std::endl;
    std::cout << "  iteration made: " << stab_iteration << std::endl;
    return stab_iteration < 3;
}

void Algorithm::update_if_needed() {
    if (!update_needed()) {
        return;
    }
    while (!updated_phi.empty()) {
        std::set<int>::iterator it = updated_phi.begin();
        Phi[*it].update(Phi_zero, PhiParam);
        updated_phi.erase(it);
    }
    while (!updated_theta.empty()) {
        std::set<int>::iterator it = updated_theta.begin();
        Theta[*it].update(Theta_zero, ThetaParam);
        updated_theta.erase(it);
    }
}

bool Algorithm::get_next_word(int & document, int & wordIdx) const {
    if (wordIdx + 1 < docsWords.unique_words_number(document)) {
        ++wordIdx;
    } else {
        wordIdx = 0;
        ++document;
        //std::cout << "d#" << document << " " << std::endl << *this;
        std::cout << "d#" << document << " ";
    }
    return (document < docsWords.docs_number());
}


void Algorithm::perform() {
    clock_t zeroTime = clock();
    //std::cout << (*this);
    do {
        int d = 0, w = -1;
        while (get_next_word(d, w)) {
            // w is index, it is NOT a real word number
            prepareDocWord(d, w);
            //std::cout << d << " "  << w << std::endl;
            for (size_t i = 0; i < HiddenNumbers[d][w].size(); ++i) {
                prepareTopic(d, w, i);
                int topic = HiddenNumbers[d][w][i].id;
                double delta = getDelta(d,w,topic);
                if (delta != delta) {
                    delta = 0;
//                    std::cout << "getDelta. d=" << d << " w=" << w << " delta=" << delta << std::endl;
//                    exit(-6);
                }
                HiddenNumbers[d][w][i].counter += delta;
                if (!Phi[topic].inc(docsWords.word_id(d, w), delta)) {
                    std::cout << "phi.inc. t=" << topic << " d=" << d <<" w=" << w << " w_id=" << docsWords.word_id(d, w) << " " << delta << std::endl;
                    exit(-6);
                };
                updated_phi.insert(topic);
                if (!Theta[d].inc(topic, delta)) {
                    std::cout << "theta.inc" << d << " " << topic << " " << delta << std::endl;
                    exit(-6);
                };
                updated_theta.insert(d);
            }
            update_if_needed();
        }
    } while (not_stabilised());
    std::cout <<std::endl<< "Algorithm PERFORMED. Total time is "
              << (float)(clock() - zeroTime) / CLOCKS_PER_SEC << std::endl;
}

std::ostream & operator << (std::ostream & out,
                            const Algorithm & algorithm) {
    out << "Phi  ---  topic -> words" << std::endl;
    for (int topic = 0; topic < static_cast<int>(algorithm.Phi.size()); ++topic) {
        out << "t" << topic << " : ";
        std::vector<Pair> top(algorithm.Phi[topic].top(5));
        for (size_t pair = 0; pair < top.size(); ++pair) {
            out << top[pair];
        }
        out << std::endl;
    }
    out << std::endl;

    out << "Theta  ---  document -> topic" << std::endl;
    for (int topic = 0; topic < static_cast<int>(algorithm.Theta.size()); ++topic) {
        out << "d" << topic << " : ";
        std::vector<Pair> top(algorithm.Theta[topic].top(2));
        for (size_t pair = 0; pair < top.size(); ++pair) {
            out << top[pair];
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}

std::ostream & Algorithm::print(std::ostream & out, const Dictionary & dict) const {
    out << "Phi" << std::endl;
    for (int t = 0; t < static_cast<int>(Phi.size()); ++t) {
        out << "t" << t << " :";
        std::vector<Pair> top(Phi[t].top(20));
        for (size_t i = 0; i < top.size(); ++i) {
            out << " " << dict.word(top[i].id) << " " << top[i].counter;
        }
        out << std::endl;
    }
    out << std::endl;

    out << "Theta" << std::endl;
    for (int d = 0; d < static_cast<int>(Theta.size()); ++d) {
        out << "d" << d << " :";

        for (int t = 0; t < Theta[d].size(); ++t) {
            out << " " << Theta[d](t) ;
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}

