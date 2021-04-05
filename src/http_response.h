//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_HTTP_RESPONSE_H
#define P8_WEB_SERVER_HTTP_RESPONSE_H

#include <string>
#include <map>

class HTTPResponse {
public:
    HTTPResponse() {}

    HTTPResponse(int code, const std::string& status, const std::string& version) {
        this->code = code;
        this->status = status;
        this->version = version;
        body = NULL;
    }

    static HTTPResponse OK() {
        return HTTPResponse(200, "OK", "HTTP/1.1");
    }

    int code;
    std::string version;
    std::string status;
    std::map<std::string, std::string> headers;
    std::string* body;

    std::string* generate();

    static HTTPResponse* parse(const std::string*);

    template <typename T>
    static int parse_header(T start, T end, HTTPResponse *response) {
        auto cursor = start;
        auto mark = cursor;
        std::vector<std::string> words;
        int c = 0;
        for (; cursor != end; cursor++) {
            if (*cursor == '\r')
                break;
            if (c < 2 && isspace(*cursor)) {
                words.emplace_back(mark, cursor);
                mark = cursor + 1;
                c++;
            }
        }
        response->version = words[0];
        response->code = atoi(words[1].c_str());
        response->status = std::string(mark, cursor);

        while (cursor != end && !isalnum(*cursor)) cursor++;

        while (cursor != end) {
            auto beginning = cursor;
            while (cursor != end && *cursor != ':') cursor++;
            auto key = std::string(beginning, cursor);
            beginning = cursor + 1;
            while (isspace(*beginning)) beginning++;
            while (cursor != end && *cursor != '\r') cursor++;
            response->headers[key] = std::string(beginning, cursor);
            int dist = 0;
            while (cursor != end && !isalnum(*cursor)) {
                dist++;
                cursor++;
            }
            if (dist == 4)
                break;
        }

        return cursor - start;
    }

    template <typename T>
    static void parse_body(T start, HTTPResponse *response) {
        if (response->headers.find("Content-Length") != response->headers.end()) {
            int length = atoi(response->headers["Content-Length"].c_str());
            response->body = new std::string(start, start + length);
        }
    }

    static int parse_chunk_header(const std::string&, int, int&);
    static int parse_chunk(const std::string&, int, int, HTTPResponse*);
};


#endif //P8_WEB_SERVER_HTTP_RESPONSE_H
