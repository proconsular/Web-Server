//
// Created by Chris Luttio on 3/24/21.
//

#ifndef P8_WEB_SERVER_UTILS_H
#define P8_WEB_SERVER_UTILS_H

#include <vector>
#include <string>

std::vector<std::string> split_string(const std::string& separator, const std::string& str);
std::string join(const std::string& separator, const std::vector<std::string>& arr);
int32_t decode_hex_str(const std::string&);

#endif //P8_WEB_SERVER_UTILS_H
