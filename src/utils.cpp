//
// Created by Chris Luttio on 3/25/21.
//

#include "utils.h"
#include <math.h>
#include <map>
#include <algorithm>
#include <openssl/ssl.h>
#include <openssl/err.h>

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

std::chrono::high_resolution_clock::time_point get_time() {
    return std::chrono::high_resolution_clock::now();
}

uint32_t get_millisecond_duration(std::chrono::high_resolution_clock::time_point a, std::chrono::high_resolution_clock::time_point b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(a - b).count();
}

uint32_t get_ms_to_now(std::chrono::high_resolution_clock::time_point t) {
    return get_millisecond_duration(get_time(), t);
}

std::vector<std::string> parse_accept_header(const std::string& header) {
    std::map<float, std::map<std::string, std::vector<std::string>>> data;

    float quality = 1;

    std::string primary, secondary;
    auto i = header.cbegin();
    while (*i == ' ') i++;
    auto s = i;
    for (; i != header.cend();) {
        if (*i == '/') {
            primary = std::string(s, i);
            ++i;
            s = i;
        } else if (*i == ';' || *i == ',') {
            if (s < i)
                secondary = std::string(s, i);
            if (*i == ',') {
                if (data.find(quality) == data.end()) {
                    data[quality] = std::map<std::string, std::vector<std::string>>();
                    if (data[quality].find(primary) == data[quality].end()) {
                        data[quality][primary] = std::vector<std::string>();
                    }
                }
                data[quality][primary].push_back(secondary);
                ++i;
                while (*i == ' ') i++;
                s = i;
            } else if (*i == ';') {
                while (*i != '=') i++;
                ++i;
                auto e = i;
                while (e != header.cend() && *e != ',') e++;
                auto str = std::string(i, e);
                quality = atof(str.c_str());
                i = e;
                s = i + 1;
            }
        } else {
            i++;
        }
    }

    if (s < i)
        secondary = std::string(s, i);
    if (data.find(quality) == data.end()) {
        data[quality] = std::map<std::string, std::vector<std::string>>();
        if (data[quality].find(primary) == data[quality].end()) {
            data[quality][primary] = std::vector<std::string>();
        }
    }
    data[quality][primary].push_back(secondary);

    std::vector<std::string> list;

    for (auto iter = data.crbegin(); iter != data.crend(); iter++) {
        auto primes = iter->second;
        std::vector<std::string> keys;
        keys.reserve(primes.size());
        for (const auto& p : primes)
            keys.push_back(p.first);
        std::sort(keys.begin(), keys.end(), [](const std::string& a, const std::string& b) {
            if (a != "*" && b == "*")
                return true;
            if (a == "*" && b != "*")
                return false;
            return true;
        });
        for (const auto & k : keys) {
            auto seconds = primes[k];
            std::sort(seconds.begin(), seconds.end(), [](const std::string& a, const std::string& b) {
                if (a != "*" && b == "*")
                    return true;
                if (a == "*" && b != "*")
                    return false;
                return true;
            });
            for (const auto & s: seconds) {
                list.push_back(k + "/" + s);
            }
        }
    }

    return list;
}

bool compare_routes(const std::string& a, const std::string& b) {
    uint8_t score[2] = {0, 0};
    std::string routes[2] = {a, b};
    for (int i = 0; i < 2; i++) {
        auto url = routes[i];
        for (int k = 0; k < url.size(); k++) {
            char c = url[k];
            if (c == '*') {
                score[i] += 1;
            } else if (c == '/') {
                score[i] += 2;
            } else {
                int j = k;
                while (k < url.size() && url[k] != '*' && url[k] != '/') k++;
                if (j < k) {
                    score[i] += 3;
                }
                if (k < url.size()) {
                    k--;
                }
            }
        }
    }
    return score[0] < score[1];
}

bool route_applies(const std::string& route, const std::string& url) {
    int i = 0;
    for (; i < route.size() && i < url.size(); i++) {
        char r = route[i];
        char u = url[i];
        if (r != u) {
            if (r != '*') {
                return false;
            }
            return true;
        }
    }
    return i == url.size() && route.size() <= url.size();
}

std::string get_ssl_str_err() {
    BIO *bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf;
    size_t len = BIO_get_mem_data(bio, &buf);
    std::string ret(buf, len);
    BIO_free(bio);
    return ret;
}

std::string encode_base_64(const uint8_t *str, size_t len) {
    std::string output;

    for (int i = 0; i < len; i += 3) {
        uint8_t a = str[i];
        uint8_t b = i + 1 < len ? str[i + 1] : 0;
        uint8_t c = i + 2 < len ? str[i + 2] : 0;

        uint8_t w = (a >> 2) & 0x3f;
        uint8_t x = ((a & 0b11) << 4) | (b >> 4);
        uint8_t y = ((b & 0b1111) << 2) | (c >> 6);
        uint8_t z = c & 0b111111;

        if (b == 0) {
            y = 64;
        }

        if (c == 0) {
            z = 64;
        }

        uint8_t buf[4] = {w, x, y, z};
        for (int n = 0; n < 4; n++) {
            uint8_t v = buf[n];
            if (v < 26)
                buf[n] = 'A' + v;
            else if (v < 52)
                buf[n] = 'a' + v - 26;
            else if (v < 62)
                buf[n] = '0' + v - 52;
            else if (v == 62)
                buf[n] = '+';
            else if (v == 63)
                buf[n] = '/';
            else if (v == 64)
                buf[n] = '=';
            output.push_back((char)(buf[n]));
        }
    }

    return output;
}

uint32_t convert_from_big_endian(const char* bytes, uint8_t len) {
    uint32_t num = 0;
    for (int i = 0; i < len; i++) {
        num += bytes[len - i] * pow(255, i);
    }
    return num;
}