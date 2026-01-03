//
// Created by Abdullah Faiz ur rahman on 23/12/2025.
//

#include "MyUtility.h"


std::string MyUtility::read_file(std::string filename) {
    std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);
    if(!file.is_open())
        throw std::runtime_error("Failed to open " + filename);
    using Iterator = std::istreambuf_iterator<char>;
    std::string content(Iterator{file}, Iterator{});
    if(!file)
        throw std::runtime_error("Failed to read " + filename);
    return content;
}
