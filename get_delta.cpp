#include "get_delta.h"
//#include "hidden.h"
#include "func.h"
#include <iostream>

double LDA_GetDelta::operator() (int document,
                                 int word_id, int word_counter, int hidden_idx,
                                 const Hidden_Word & hidden_word,
                                 const PhiTheta & phi_theta,
                                 double Z) {
    return 1;
}

double PLSA_GetDelta::operator() (int document,
                                  int word_id, int word_counter, int hidden_idx,
                                  const Hidden_Word & hidden_word,
                                  const PhiTheta & phi_theta,
                                  double Z)  {
    const std::pair<int, double> & topic = hidden_word.topic[hidden_idx];
    if ( (phi_theta.phi(topic.first, word_id) > 0)
                && (phi_theta.theta(document, topic.first) > 0)) {
        return word_counter * phi_theta.phi(topic.first, word_id)
               * phi_theta.theta(document, topic.first) / Z
               - topic.second;
    } else {
        return 0;
    }
}
