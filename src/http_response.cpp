//
// Created by Chris Luttio on 3/25/21.
//

#include <vector>
#include <iostream>
#include "utils.h"
#include "http_response.h"

HTTPResponse* HTTPResponse::parse(const std::string *data) {
    auto* response = new HTTPResponse;

//    auto cursor = data->begin();
//    auto mark = cursor;
//    std::vector<std::string> words;
//    for (; cursor != data->end(); cursor++) {
//        if (*cursor == '\r')
//            break;
//        if (isspace(*cursor)) {
//            words.emplace_back(mark, cursor);
//            mark = cursor + 1;
//        }
//    }
//    response->version = words[0];
//    response->code = atoi(words[1].c_str());
//    response->status = std::string(mark, cursor);
//
//    while (cursor != data->end() && !isalnum(*cursor)) cursor++;
//
//    while (cursor != data->end()) {
//        auto start = cursor;
//        while (cursor != data->end() && *cursor != ':') cursor++;
//        auto key = std::string(start, cursor);
//        start = cursor + 1;
//        while (isspace(*start)) start++;
//        while (cursor != data->end() && *cursor != '\r') cursor++;
//        response->headers[key] = std::string(start, cursor);
//        int dist = 0;
//        while (cursor != data->end() && !isalnum(*cursor)) {
//            dist++;
//            cursor++;
//        }
//        if (dist == 4)
//            break;
//    }

    auto cursor = data->begin() + parse_header(data->begin(), data->end(), response);

    if (response->headers.find("Content-Length") != response->headers.end()) {
        int length = atoi(response->headers["Content-Length"].c_str());
        response->body = new std::string(cursor, cursor + length);
    }

    return response;
}

std::string* HTTPResponse::generate() {
    std::vector<std::string> request_line = {version, std::to_string(code), status};

    std::vector<std::string> headers_output;
    for (std::map<std::string, std::string>::const_iterator i = headers.begin(); i != headers.end(); i++) {
        headers_output.push_back(join(": ", {i->first, i->second}));
    }

    int body_size = 0;
    if (body != NULL) {
        body_size = body->size();
    }
    headers_output.push_back(join(": ", {"Content-Length", std::to_string(body_size)}));

    return new std::string(join("\r\n", {join(" ", request_line), join("\r\n", headers_output), "", body != NULL ? *body : ""}));
}

//template <class T>
//int HTTPResponse::parse_header(T start, T end, HTTPResponse *response) {
//    auto cursor = start;
//    auto mark = cursor;
//    std::vector<std::string> words;
//    for (; cursor != end; cursor++) {
//        if (*cursor == '\r')
//            break;
//        if (isspace(*cursor)) {
//            words.emplace_back(mark, cursor);
//            mark = cursor + 1;
//        }
//    }
//    response->version = words[0];
//    response->code = atoi(words[1].c_str());
//    response->status = std::string(mark, cursor);
//
//    while (cursor != end && !isalnum(*cursor)) cursor++;
//
//    while (cursor != end) {
//        auto beginning = cursor;
//        while (cursor != end && *cursor != ':') cursor++;
//        auto key = std::string(beginning, cursor);
//        beginning = cursor + 1;
//        while (isspace(*beginning)) beginning++;
//        while (cursor != end && *cursor != '\r') cursor++;
//        response->headers[key] = std::string(beginning, cursor);
//        int dist = 0;
//        while (cursor != end && !isalnum(*cursor)) {
//            dist++;
//            cursor++;
//        }
//        if (dist == 4)
//            break;
//    }
//
//    return cursor - start;
//}

//template <class T>
//void HTTPResponse::parse_body(T start, HTTPResponse *response) {
//    if (response->headers.find("Content-Length") != response->headers.end()) {
//        int length = atoi(response->headers["Content-Length"].c_str());
//        response->body = new std::string(start, start + length);
//    }
//}

int HTTPResponse::parse_chunk_header(const std::string& str, int start, int &size) {
    auto end = start;
    while (end < str.size() && str[end] != '\r') end++;
    if (end >= str.size() || str[end] != '\r')
        return -1;
    size = decode_hex_str(std::string(str.begin() + start, str.begin() + end));
    return end + 2;
}

int HTTPResponse::parse_chunk(const std::string& str, int start, int size, HTTPResponse *response) {
    if (response->body == nullptr) {
        response->body = new std::string;
    }
    response->body->append(std::string(str.begin() + start, str.begin() + start + size));
    return start + size + 2;
}