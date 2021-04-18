//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_HTTP_RESPONSE_H
#define P8_WEB_SERVER_HTTP_RESPONSE_H

#include <string>
#include <map>
#include <vector>
#include <memory>

class HTTPResponse {
public:
    explicit HTTPResponse() {}

    HTTPResponse(int code, const std::string& status, const std::string& version) {
        this->code = code;
        this->status = status;
        this->version = version;
        body = nullptr;
    }

    static HTTPResponse OK() {
        return HTTPResponse(200, "OK", "HTTP/1.1");
    }

    void setHeader(const std::string& key, const std::string& value) {
        headers[key] = std::make_shared<std::string>(value);
    }

    int code;
    std::string version;
    std::string status;
    std::map<std::string, std::shared_ptr<std::string>> headers;
    std::shared_ptr<std::string> body;

    [[nodiscard]] std::shared_ptr<std::string> generate() const;

    static std::shared_ptr<HTTPResponse> parse(const std::shared_ptr<std::string>&);

    template <typename T>
    static int parse_status_line(T start, T end, const std::shared_ptr<HTTPResponse>& response) {
        auto cursor = start;
        auto mark = cursor;
        std::vector<std::string> words;
        int c = 0;
        for (; cursor != end; cursor++) {
            if (*cursor == '\r' || *cursor == '\n')
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
        return cursor - start;
    }

    template<typename T>
    static int parse_header(T start, T end, const std::shared_ptr<HTTPResponse>& response) {
        auto cursor = start;
        auto beginning = cursor;
        while (cursor != end && *cursor != ':') cursor++;
        if (cursor == end) {
            return -1;
        }
        auto key = std::string(beginning, cursor);
        beginning = cursor + 1;
        while (isspace(*beginning)) beginning++;
        while (cursor != end && *cursor != '\0' && *cursor != '\r' && *cursor != '\n') cursor++;
        if (cursor == end || *cursor == '\0') {
            return -1;
        }
        response->headers[key] = std::make_shared<std::string>(beginning, cursor);
        if ((*cursor == '\n' && cursor + 1 < end) || (*cursor == '\r' && cursor + 2 < end)) {
            cursor += *cursor == '\n' ? 1 : 2;
        } else {
            return -1;
        }
        return cursor - start;
    }

    template <typename T>
    static int parse_headers(T start, T end, const std::shared_ptr<HTTPResponse>& response) {
        auto cursor = start;
        while (cursor != end && *cursor != '\0') {
            auto beginning = cursor;
            while (cursor != end && *cursor != ':') cursor++;
            auto key = std::string(beginning, cursor);
            beginning = cursor + 1;
            while (isspace(*beginning)) beginning++;
            while (cursor != end && *cursor != '\0' && *cursor != '\r' && *cursor != '\n') cursor++;
            response->headers[key] = std::make_shared<std::string>(beginning, cursor);
            int dist = 0;
            char last_char = '\0';
            while (cursor != end && !isalnum(*cursor)) {
                last_char = *cursor;
                dist++;
                cursor++;
                if (cursor != end && (last_char == '\n' && *cursor == '\n'))
                    break;
            }
            if ((last_char == '\n' && *cursor == '\n')) {
                cursor++;
                break;
            }
            if (dist == 4)
                break;
        }

        return cursor - start;
    }

    template <typename T>
    static void parse_body(T start, const std::shared_ptr<HTTPResponse>& response) {
        if (response->headers.find("Content-Length") != response->headers.end()) {
            int length = atoi(response->headers["Content-Length"]->c_str());
            response->body = std::make_shared<std::string>(start, start + length);
        }
    }

    static int parse_chunk_header(const std::shared_ptr<std::string>&, int, int&);
    static int parse_chunk(const std::shared_ptr<std::string>&, int, int, const std::shared_ptr<HTTPResponse>&);
};


#endif //P8_WEB_SERVER_HTTP_RESPONSE_H
