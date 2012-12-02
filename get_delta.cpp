#include "get_delta.h"
#include <iostream>

double LDA_GetDelta::operator() (int document,
                                 int word_id, int word_counter, int hidden_idx,
                                 const std::vector<std::pair<int, double> > & topics,
                                 const PhiTheta & phi_theta) {
    return 1;
}

/*PLSA_GetDelta::PLSA_GetDelta():
    ready_word_id(-1),
    ready_doc(-1) {
}*/

double PLSA_GetDelta::operator() (int document,
                                  int word_id, int word_counter, int hidden_idx,
                                  const std::vector<std::pair<int, double> > & topics,
                                  const PhiTheta & phi_theta)  {
    if ((ready_word_id != word_id) || (ready_doc != document)) {
        ready_word_id = word_id;
        ready_doc = document;
        Z = 0;
        //std::cout << "updZ ";
        for (size_t i = 0; i < topics.size(); ++i) {
            Z += phi_theta.phi(topics[i].first, word_id)
                 * phi_theta.theta(document, topics[i].first);
        }
        //std::cout << Z <<" |";
    } // caching Z

    if ((topics[hidden_idx].second > 0)
            || ((phi_theta.phi(topics[hidden_idx].first, word_id) > 0)
                && (phi_theta.theta(document, topics[hidden_idx].first) > 0)) ) {
        //      std::cout << "|NOT_null: " << word_counter << " "<<  phi_theta.phi(topics[hidden_idx].first, word_id) << " " << phi_theta.theta(document, topics[hidden_idx].first) << " "<<Z <<"|";
        return word_counter * phi_theta.phi(topics[hidden_idx].first, word_id)
               * phi_theta.theta(document, topics[hidden_idx].first) / Z
               - topics[hidden_idx].second;
    } else {
        return 0;
    }
}
