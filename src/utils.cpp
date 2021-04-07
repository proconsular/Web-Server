//
// Created by Chris Luttio on 3/25/21.
//

#include "utils.h"
#include <math.h>
#include <map>

std::vector<std::string> split_string(const std::string& separator, const std::string& str) {
    std::vector<std::string> parts;

    std::string::const_iterator start_word = str.begin();
    std::string::const_iterator iter = start_word;
    std::string::const_iterator start_separator = start_word;
    for (; iter != str.end(); iter++) {
        std::string word(start_separator, iter);
        if (word == separator) {
            parts.emplace_back(start_word, start_separator);
            start_separator = iter;
            start_word = iter;
        }
        if (*iter == *separator.begin()) {
            start_separator = iter;
        }
    }

    if (start_word < iter) {
        if (start_word < start_separator && start_separator < iter && iter - start_separator == separator.size()) {
            iter = start_separator;
        }
        parts.emplace_back(start_word, iter);
    }

    return parts;
}

std::string join(const std::string& separator, const std::vector<std::string>& arr) {
    std::string output;

    for (std::vector<std::string>::const_iterator i = arr.begin(); i != arr.end(); i++) {
        output.append(*i);
        if (i + 1 < arr.end()) {
            output.append(separator);
        }
    }

    return output;
}

int32_t decode_hex_str(const std::string& str) {
    int32_t num = 0;

    for (auto iter = str.begin(); iter != str.end(); iter++) {
        int place = str.end() - iter;
        int c = tolower(*iter);
        int digit = 0;
        if ('0' <= c && c <= '9') {
            digit = c - '0';
        } else if ('a' <= c && c <= 'f') {
            digit = c - 'a' + 10;
        }
        num += digit * (pow(16, place - 1));
    }

    return num;
}

std::string generate_hash_id(int length) {
    std::string output;

    for (int i = 0; i < length; i++) {
        output.push_back('!' + rand() % 78);
    }

    return output;
}