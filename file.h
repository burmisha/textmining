#ifndef FILE_H
#define FILE_H

#include "header.h"

class fileHandler {
        const string prefix;
        const string extension;
        int CurrentFile;
    public:
		string NewFileName();
        bool areMoreFiles();
        fileHandler();
        int CurrentFileID();
};

#endif
