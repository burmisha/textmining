#include <algorithm>

#include "phi_theta.h"
#include "stdlib.h"
void HyperParameter::inc(int first_idx, int second_idx, double delta) {
    if (!fixed) {
        LatinLetter[first_idx].inc(second_idx, delta, gamma[second_idx]);
        updated_indexes.insert(first_idx);
    }
}

void HyperParameter::set_fixed(bool fixed) {
   this->fixed = fixed;
}

void HyperParameter::update() {
    if (!fixed) {
        while (!updated_indexes.empty()) {
            std::set<int>::iterator it = updated_indexes.begin();
            LatinLetter[*it].update(gamma_sum, gamma);
            updated_indexes.erase(it);
        }
    }
}

Discrete & HyperParameter::operator[](int i) {
    return LatinLetter[i];
}

const Discrete & HyperParameter::operator[](int i) const {
    return LatinLetter[i];
}

HyperParameter::HyperParameter(int size, const std::vector<double> & gamma):
    LatinLetter(size, Discrete(gamma.size())),
    gamma(gamma), gamma_sum(0), fixed(false) {
    for (size_t i = 0; i < this->gamma.size(); ++i) {
        gamma_sum += (this->gamma)[i];
    }
}

std::vector<double> HyperParameter::gamma_parameters() const {
    return gamma;
}

std::vector<std::vector<std::pair<int, double> > > HyperParameter::top(int size) const {
    std::vector<std::vector<std::pair<int, double> > > top;
    for (size_t i = 0; i < LatinLetter.size(); ++i) {
        top.push_back(LatinLetter[i].top(size));
    }
    return top;
}

void PhiTheta::set_phi_fixed(bool fixed) {
    Phi.set_fixed(fixed);
}

void PhiTheta::set_theta_fixed(bool fixed) {
    Theta.set_fixed(fixed);
}

PhiTheta::PhiTheta(const DocsWords & docs_words, const Distance & distance,
                   const std::vector<double> & alpha, const std::vector<double> & beta):
    Phi(beta.size(), alpha), Theta(docs_words.docs_number(), beta) {
    const int topic_number = beta.size();
    std::vector<double> words_counter(docs_words.unique_words_number(), 0);
    for (int t = 0; t < topic_number; ++t) {
        int d = rand() * docs_words.docs_number() / RAND_MAX;
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            Phi.inc(t, docs_words.word_id(d, w), docs_words.word_counter(d, w));
            words_counter[docs_words.word_id(d, w)] += docs_words.word_counter(d, w);
        }
        Phi.update();
    }

    const int R = 3 * topic_number;
    for (int r = 0; r < R; ++r) {
        int d = rand() * docs_words.docs_number() / RAND_MAX;

        /* FIND BEST TOPIC */
        Discrete words_distribution(docs_words.unique_words_number()); // $$\hat p_d$$
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            words_distribution.inc(docs_words.word_id(d, w), docs_words.word_counter(d, w), 1);
        }
        words_distribution.update(); // NEW !!!
        int best_topic = 0;
        double min_distance = distance(Phi[0], words_distribution);
        for (int t = 1; t < topic_number; ++t) {
            if (distance(Phi[t], words_distribution) < min_distance) {
                best_topic = t;
            }
        }

        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            Phi.inc(best_topic, docs_words.word_id(d, w), docs_words.word_counter(d, w));
            words_counter[docs_words.word_id(d, w)] += docs_words.word_counter(d, w);
        }
        Phi.update();
    }

    for (int d = 0; d < docs_words.docs_number(); ++d) {
        for (int t = 0; t < topic_number; ++t) {
            for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
                double add = static_cast<double>(docs_words.word_counter(d, w))
                             * Phi[t].get_counter(docs_words.word_id(d,w))
                             / docs_words.total_words_number(d)
                             / words_counter[docs_words.word_id(d, w)];
                if (add != add) {
                    /*std::cout << d << " " << t << " " << w << " " <<  docs_words.total_words_number(d) << " " << docs_words.word_id(d, w) <<" "  << words_counter[docs_words.word_id(d, w)];
                    system("pause");
                    exit(5);*/
                } else {
                    Theta[d].add_prob(t, add);
                }
            }
        }
    }

    /* REGULARIZE */
    const double small = 0.05;
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        Theta[d].regularize(small);
    }
    for (int t = 0; t < topic_number; ++t) {
        Phi[t].regularize(small);
    }
}

void PhiTheta::inc(int document, int word_id, int topic, double delta) {
    Phi.inc(topic, word_id, delta);
    Theta.inc(document, topic, delta);
}

double PhiTheta::phi(int i, int j) const {
    return Phi[i](j);
}

double PhiTheta::theta(int i, int j) const {
    return Theta[i](j);
}

int PhiTheta::topic_number() const {
    return Theta[0].size();
}

void PhiTheta::update() {
    Phi.update();
    Theta.update();
}

PhiTheta::PhiTheta(const HyperParameter & Phi, const HyperParameter & Theta):
    Phi(Phi), Theta(Theta) {
}

PhiTheta PhiTheta::save_phi(const DocsWords & docs_words) const {
    PhiTheta phi_theta(this->Phi, HyperParameter(docs_words.docs_number(), (this->Theta).gamma_parameters()));

    std::vector<double> words_counter(docs_words.unique_words_number(), 0);
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
            words_counter[docs_words.word_id(d, w)] += docs_words.word_counter(d, w);
        }
    }

    for (int d = 0; d < docs_words.docs_number(); ++d) { // TODO: Duplication of code! see constructor
        for (int t = 0; t < topic_number(); ++t) {
            for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
                double add = static_cast<double>(docs_words.word_counter(d, w))
                             * Phi[t].get_counter(docs_words.word_id(d,w))
                             / docs_words.total_words_number(d)
                             / words_counter[docs_words.word_id(d, w)];
                if (add == add) {
                    phi_theta.Theta[d].add_prob(t, add);
                }
            }
        }
    }

    const double small = 0.05;
    for (int d = 0; d < docs_words.docs_number(); ++d) {
        phi_theta.Theta[d].regularize(small);
    }

    return phi_theta;
}

std::ostream & PhiTheta::print(std::ostream & out, const Dictionary & dict) const {
    out << "Phi" << std::endl;
    std::vector<std::vector<std::pair<int, double> > > top_phi(Phi.top(20));
    for (size_t t = 0; t < top_phi.size(); ++t) {
        out << "t" << t << " :";
        for (size_t i = 0; i < top_phi[t].size(); ++i) {
            out << " " << dict.word(top_phi[t][i].first) << " " << top_phi[t][i].second;
        }
        out << std::endl;
    }
    out << std::endl;

    out << "Theta" << std::endl;
    std::vector<std::vector<std::pair<int, double> > > all_theta(Theta.top(topic_number()));
    for (size_t d = 0; d < all_theta.size(); ++d) {
        out << "d" << d << " :";
        std::sort(all_theta[d].begin(), all_theta[d].end());
        for (size_t t = 0; t < all_theta[d].size(); ++t) {
            out << " " << all_theta[d][t].second;
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}
