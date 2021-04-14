//
// Created by Chris Luttio on 3/25/21.
//

#include <vector>
#include <iostream>
#include "utils.h"
#include "http_response.h"

std::shared_ptr<HTTPResponse> HTTPResponse::parse(const std::shared_ptr<std::string>& data) {
    auto response = std::make_shared<HTTPResponse>();

    auto status_end = parse_status_line(data->begin(), data->end(), response);
    auto headers_end = parse_headers(data->begin() + status_end, data->end(), response);
    auto cursor = data->begin() + status_end + headers_end;

    if (response->headers.find("Content-Length") != response->headers.end()) {
        int length = atoi(response->headers["Content-Length"]->c_str());
        response->body = std::make_shared<std::string>(cursor, cursor + length);
    }

    return response;
}

std::shared_ptr<std::string> HTTPResponse::generate() const {
    std::vector<std::string> request_line = {version, std::to_string(code), status};

    std::vector<std::string> headers_output;
    for (const auto & header : headers) {
        headers_output.push_back(join(": ", {header.first, *header.second}));
    }

    int body_size = 0;
    if (body != nullptr) {
        body_size = body->size();
    }
    headers_output.push_back(join(": ", {"Content-Length", std::to_string(body_size)}));

    return std::make_shared<std::string>(join("\r\n", {join(" ", request_line), join("\r\n", headers_output), "", body != nullptr ? *body : ""}));
}

int HTTPResponse::parse_chunk_header(const std::shared_ptr<std::string>& str, int start, int &size) {
    auto end = start;
    while (end < str->size() && str->at(end) != '\r' && str->at(end) != '\0') end++;
    if (end >= str->size() || str->at(end)  != '\r')
        return -1;
    if (str->at(end)  == '\r' && end + 1 >= str->size())
        return -1;
    size = decode_hex_str(std::string(str->begin() + start, str->begin() + end));
    return end - start + 2;
}

int HTTPResponse::parse_chunk(const std::shared_ptr<std::string>& str, int start, int size, const std::shared_ptr<HTTPResponse>& response) {
    if (response->body == nullptr) {
        response->body = std::make_shared<std::string>();
    }
    response->body->append(std::string(str->begin() + start, str->begin() + start + size));
    return start + size + 2;
}