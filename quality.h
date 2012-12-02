#ifndef QUALITY_H
#define QUALITY_H

class Quality {
	FileHandler file_handler;

    std::pair<DocsWords, DocsWords> control(docs_words.build_control(dictionary , control_files));
    DocsWords train = control.first, test = control.second;

    

    phi_theta.set_phi_fixed(true);
    update = update_every(100);
    stabilized = stabilized_after(5);
    Algorithm check(train, phi_theta, hidden, prepare, get_delta, update, stabilized);
};

#endif