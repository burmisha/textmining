#include "file.h"

string fileHandler::NewFileName() {
    ++CurrentFile;
    std::stringstream number;
    number << CurrentFile;
	return prefix + number.str() + extension;

};
bool  fileHandler::areMoreFiles() {
    return CurrentFile <7146; //7164
};
fileHandler::fileHandler():
    prefix("d:\\base_main\\AlexRom\\final\\text "),
    extension(".txt"),
    CurrentFile(0) {
};
int fileHandler::CurrentFileID() {
    //cout << CurrentFile << "||";
    return CurrentFile;
}

