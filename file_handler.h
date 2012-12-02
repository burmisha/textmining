#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <fstream>

class FileHandler {
private:
    const std::string prefix;
    const std::string extension;
    int min_file_number;
    int max_file_number;
    int current_file;
public:
    FileHandler(const std::string & prefix, const std::string & extension,
                int min_file_number, int max_file_number);
    std::string filename() const;
    std::string short_filename() const;
    void drop();
    bool goto_next();
};

#endif
