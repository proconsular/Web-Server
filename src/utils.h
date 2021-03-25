//
// Created by Chris Luttio on 3/24/21.
//

#ifndef P8_WEB_SERVER_UTILS_H
#define P8_WEB_SERVER_UTILS_H

#include <vector>
#include <string>

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

#endif //P8_WEB_SERVER_UTILS_H
