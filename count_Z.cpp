#include "count_Z.h"

double count_Z(int document, int word_id,
	const Hidden_Word & hidden_word,
	const PhiTheta & phi_theta) {
	double Z = hidden_word.pi;
		for (size_t i = 0; i < hidden_word.topic.size(); ++i) {
			Z += phi_theta.phi(hidden_word.topic[i].first, word_id)
			* phi_theta.theta(document, hidden_word.topic[i].first);
	}
	return Z;
}
