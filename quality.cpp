#include "quality.h"
#include <math.h>

double count_quality(const DocsWords & docs_words,
					 const PhiTheta & phi_theta,
					 const Distance & distance) {

	int n = 0;
	double sum = 0;
	for (int d = 0; d < docs_words.docs_number(); ++d) {
		int n_d = docs_words.total_words_number(d);
		Discrete p_wd(docs_words.unique_words_number());
		Discrete hat_p_wd(docs_words.unique_words_number());
		for (int w = 0; w < docs_words.unique_words_number(d); ++w) {
			int word_id = docs_words.word_id(d, w);
			p_wd.inc(word_id, docs_words.word_counter(d, w));
			for (int t = 0; t < phi_theta.topic_number(); ++t) {
				hat_p_wd.inc(word_id, phi_theta.phi(t, word_id) * phi_theta.theta(d, t));
			}
		}
		p_wd.update(); 		// p_wd is wormed
		hat_p_wd.update();
		sum += n_d * distance(p_wd, hat_p_wd);
		n += n_d;
	}
	//return exp( - sum / n);
	return (sum / n);
}
