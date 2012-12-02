#include "file.h"

string fileHandler::NewFileName() {
    ++CurrentFile;
    std::stringstream number;
    number << CurrentFile;
	return prefix + number.str() + extension;

};
bool  fileHandler::areMoreFiles() {
    return CurrentFile < 2; //7164
};
fileHandler::fileHandler():
    //prefix("d:\\base_main\\AlexRom\\test\\text "),
    prefix("d:\\base_main\\AlexRom\\test\\text "),
    extension(".txt"),
    CurrentFile(0) {
};
int fileHandler::CurrentFileID() {
    //cout << CurrentFile << "||";
    return CurrentFile;
}

