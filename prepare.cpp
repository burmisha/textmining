#include "prepare.h"
#include <algorithm>
#include "stdlib.h"
#include "func.h"
#include "iostream"

void PLSA_prepare::operator() (int document, int word_id, std::pair<int, double> & topic,
                               PhiTheta & phi_theta) {
}

void LDA_prepare::operator() (int document, int word_id, std::pair<int, double> & topic,
                              PhiTheta & phi_theta) {
    phi_theta.inc(document, word_id, topic.first, (my_round(topic.second) > 0) ? -1 : 0);

    std::vector<double> cumm_vector;
    double sum = 0;
    for (int t = 0; t < phi_theta.topic_number(); ++t) {
        sum += my_abs(phi_theta.phi(t, word_id) * phi_theta.theta(document, t));
        cumm_vector.push_back(sum);
    }
    //std::sort(cumm_vector.begin(), cumm_vector.end());
    const double rnd = (double) rand() / RAND_MAX * sum;     // scaling
    std::vector<double>::iterator it = std::lower_bound(cumm_vector.begin(), cumm_vector.end(), rnd);
    topic.first = static_cast<int>(it - cumm_vector.begin());
    if ((topic.first >= static_cast<int>(cumm_vector.size())) || (topic.first < 0)) {
        std::cout << "Look: rand\n";
        topic.first = (int) ((double) rand() / RAND_MAX * static_cast<int>(cumm_vector.size()));
    }
    topic.second = my_round(topic.second);
}
