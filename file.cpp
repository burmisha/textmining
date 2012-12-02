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
    prefix("D:\\NIR\\texts\\test\\text "),
    extension(".txt"),
    CurrentFile(0) {
};

int fileHandler::CurrentFileID() {
    return CurrentFile;
}

