//
// Created by Chris Luttio on 3/25/21.
//

#include "utils.h"

std::vector<std::string> split_string(const std::string& separator, const std::string& str) {
    std::vector<std::string> parts;

    std::string::const_iterator i = str.begin();
    std::string::const_iterator j = i;
    std::string::const_iterator n = i;
    for (; j != str.end(); j++) {
        std::string word(n, j);
        if (word == separator) {
            parts.push_back(std::string(i, n));
            n = j;
            i = j;
        }
        if (*j == *separator.begin()) {
            n = j;
        }
    }

    if (i < j) {
        parts.push_back(std::string(i, j));
    }

    return parts;
}

std::string join(const std::string& separator, const std::vector<std::string>& arr) {
    std::string output;

    for (std::vector<std::string>::const_iterator i = arr.begin(); i != arr.end(); i++) {
        output += *i;
        if (i + 1 < arr.end()) {
            output += separator;
        }
    }

    return output;
}