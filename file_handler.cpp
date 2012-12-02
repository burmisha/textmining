#include "file_handler.h"
#include <sstream>

FileHandler::FileHandler(const std::string & prefix, const std::string & extension,
                         int min_file_number, int max_file_number):
    prefix(prefix),  extension(extension),
    min_file_number(min_file_number),  max_file_number(max_file_number), current_file(min_file_number - 1) {
};

std::string FileHandler::filename() const {
    std::stringstream number;
    number << current_file;
    return prefix + number.str() + extension;
};

std::string FileHandler::short_filename() const {
    std::stringstream number;
    number << current_file;
    return number.str() + extension;
}

bool FileHandler::goto_next() {
    ++current_file;
    return current_file <= max_file_number;
};

void FileHandler::drop() {
    current_file = min_file_number - 1;
};





