#include "quality.h"

Quality::operator()(const Distance & distance) {


    FileHandler control_files(std::string("d:\\base\\AlexRom\\final\\text "),
                              std::string(".txt"), 2000 , 2100);

    std::pair<DocsWords, DocsWords> control(docs_words.build_control(dictionary , control_files));
    DocsWords train = control.first, test = control.second;
   
    PhiTheta train_phi_theta(train);
    train_phi_theta.set_phi_fixed(true);
    update = update_every(100);
    stabilized = stabilized_after(5);
    Algorithm algorithm(train, train_phi_theta, hidden, prepare, get_delta, update, stabilized);
}