//
// Created by Chris Luttio on 4/3/21.
//

#include "gtest/gtest.h"
#include "http_response.h"
#include "http_response_parser.h"
#include "utils.h"

TEST(HTTP_Response_Progressive_Parser, ParseHeader1) {
    const std::string data =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 11\r\n\r\n"
            "Hello World";
    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = status_end + HTTPResponse::parse_headers(data.begin() + status_end, data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", *response->headers["Content-Length"]);
}

TEST(HTTP_Response_Progressive_Parser, BadRequest) {
    const std::string data =
            "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\n\r\nHello World";
    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = status_end + HTTPResponse::parse_headers(data.begin() + status_end, data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(400, response->code);
    ASSERT_EQ("Bad Request", response->status);
    ASSERT_EQ("11", *response->headers["Content-Length"]);
}

TEST(HTTP_Response_Progressive_Parser, ParseHeader2) {
    const std::string data =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 11\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Encoding: 123\r\n"
            "Hello: World\r\n"
            "\r\n"
            "Hello World";
    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = status_end + HTTPResponse::parse_headers(data.begin() + status_end, data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", *response->headers["Content-Length"]);
    ASSERT_EQ("123", *response->headers["Content-Encoding"]);
    ASSERT_EQ("World", *response->headers["Hello"]);
}

TEST(HTTP_Response_Progressive_Parser, ParseFixedBody) {
    const std::string data =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 11\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Encoding: 123\r\n"
            "Hello: World\r\n"
            "\r\n"
            "Hello World";
    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = status_end + HTTPResponse::parse_headers(data.begin() + status_end, data.end(), response);
    HTTPResponse::parse_body(data.begin() + header_end, response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", *response->headers["Content-Length"]);
    ASSERT_EQ("Hello World", *response->body);
}

TEST(HTTP_Response_Progressive_Parser, HexDecoder) {
    ASSERT_EQ(11, decode_hex_str("b"));
    ASSERT_EQ(15, decode_hex_str("f"));
    ASSERT_EQ(16, decode_hex_str("10"));
    ASSERT_EQ(17, decode_hex_str("11"));
    ASSERT_EQ(25, decode_hex_str("19"));
    ASSERT_EQ(31, decode_hex_str("1f"));
}

TEST(HTTP_Response_Progressive_Parser, ParseChunkedBody) {
    auto data = std::make_shared<std::string>(
                    "HTTP/1.1 200 OK\r\n"
                    "Transfer-Encoding: chunked\r\n"
                    "Content-Type: text/plain\r\n"
                    "\r\n"
                    "b\r\n"
                    "Hello World\r\n"
                    "0\r\n"
                    );
    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(data->begin(), data->end(), response);
    int header_end = status_end + HTTPResponse::parse_headers(data->begin() + status_end, data->end(), response);
    int size1;
    auto chunk1 = HTTPResponse::parse_chunk_header(data, header_end, size1);
    HTTPResponse::parse_chunk(data, header_end + chunk1, size1, response);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ(11, size1);
    ASSERT_EQ(3, chunk1);
    ASSERT_EQ("Hello World", *response->body);
}

TEST(HTTP_Response_Progressive_Parser, ParseChunkedBody1) {
    auto data = std::make_shared<std::string>("b\r");
    int size1;
    auto chunk1 = HTTPResponse::parse_chunk_header(data, 0, size1);
    ASSERT_EQ(-1, chunk1);
}

TEST(HTTP_Response_Progressive_Parser, ParseChunkedBody2) {
    auto data = std::make_shared<std::string>("b\r\n");
    int size1;
    auto chunk1 = HTTPResponse::parse_chunk_header(data, 0, size1);
    ASSERT_EQ(3, chunk1);
}

TEST(HTTP_Response_Progressive_Parser, ParseFailedResponse1) {
    FILE *file = fopen("../res/response-1.txt", "r");

    const int buffer_size = 1024 * 4;

    char* buffer = new char[buffer_size];
    memset(buffer, 0, buffer_size);
    fread(buffer, 1, buffer_size, file);
    fclose(file);

    ASSERT_EQ("HTTP/1.1 200 OK", std::string(buffer, buffer + 15));

    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(buffer, buffer + buffer_size, response);
    int header_end = status_end + HTTPResponse::parse_headers(buffer + status_end, buffer + buffer_size, response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("nginx", *response->headers["Server"]);
    ASSERT_EQ("Sun, 11 Apr 2021 18:08:08 GMT", *response->headers["Date"]);
    ASSERT_EQ("interest-cohort=()", *response->headers["Permissions-Policy"]);
    ASSERT_EQ(
            "default-src 'none' ; connect-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; manifest-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; media-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; script-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ 'unsafe-inline' 'unsafe-eval' ; font-src data:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; img-src data:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; style-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ 'unsafe-inline' ; object-src 'none' ; worker-src blob: ; child-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; frame-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; form-action  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ https://duck.co ; frame-ancestors 'self' ; base-uri 'self' ; block-all-mixed-content ;",
            *response->headers["Content-Security-Policy"]
    );
    ASSERT_EQ("no-cache", *response->headers["Cache-Control"]);
    ASSERT_EQ("bytes", *response->headers["Accept-Ranges"]);

    HTTPResponse::parse_body(buffer + header_end, response);

    ASSERT_EQ("<!DOCTYPE html>", response->body->substr(0, 15));

    delete[] buffer;
}

TEST(HTTP_Response_Progressive_Parser, ParseFailedResponse2) {
    FILE *file = fopen("../res/response-2.txt", "r");

    const int buffer_size = 1024 * 4;

    char* buffer = new char[buffer_size];
    memset(buffer, 0, buffer_size);
    fread(buffer, 1, buffer_size, file);
    fclose(file);

    ASSERT_EQ("HTTP/1.1 200 OK", std::string(buffer, buffer + 15));

    auto response = std::make_shared<HTTPResponse>();
    int status_end = HTTPResponse::parse_status_line(buffer, buffer + buffer_size, response);
    int header_end = status_end + HTTPResponse::parse_headers(buffer + status_end, buffer + buffer_size, response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("nginx", *response->headers["Server"]);
    ASSERT_EQ("Tue, 13 Apr 2021 01:35:23 GMT", *response->headers["Date"]);
    ASSERT_EQ("interest-cohort=()", *response->headers["Permissions-Policy"]);
    ASSERT_EQ(
            "default-src 'none' ; connect-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; manifest-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; media-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; script-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ 'unsafe-inline' 'unsafe-eval' ; font-src data:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; img-src data:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; style-src  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ 'unsafe-inline' ; object-src 'none' ; worker-src blob: ; child-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; frame-src blob:  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ ; form-action  https://duckduckgo.com https://*.duckduckgo.com https://3g2upl4pq6kufc4m.onion/ https://duck.co ; frame-ancestors 'self' ; base-uri 'self' ; block-all-mixed-content ;",
            *response->headers["Content-Security-Policy"]
    );
    ASSERT_EQ("no-cache", *response->headers["Cache-Control"]);
    ASSERT_EQ("bytes", *response->headers["Accept-Ranges"]);

    delete[] buffer;
}

TEST(HTTP_Response_Progressive_Parser, ParseHeaders) {
    std::string data = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n\r\n";
    auto response = std::make_shared<HTTPResponse>();

    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = HTTPResponse::parse_header(data.begin() + status_end, data.end(), response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("keep-alive", *response->headers["Connection"]);
}

TEST(HTTP_Response_Progressive_Parser, ParseHeaders2) {
    std::string data = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: pla";
    auto response = std::make_shared<HTTPResponse>();

    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = HTTPResponse::parse_header(data.begin() + status_end, data.end(), response);
    int header_end2 = HTTPResponse::parse_header(data.begin() + status_end + header_end, data.end(), response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("keep-alive", *response->headers["Connection"]);
    ASSERT_EQ(-1, header_end2);
}

TEST(HTTP_Response_Progressive_Parser, ParseHeaders3) {
    std::string data = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Typ";
    auto response = std::make_shared<HTTPResponse>();

    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = HTTPResponse::parse_header(data.begin() + status_end, data.end(), response);
    int header_end2 = HTTPResponse::parse_header(data.begin() + status_end + header_end, data.end(), response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("keep-alive", *response->headers["Connection"]);
    ASSERT_EQ(-1, header_end2);
}

TEST(HTTP_Response_Progressive_Parser, ParseHeaders4) {
    std::string data = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text\r";
    auto response = std::make_shared<HTTPResponse>();

    int status_end = HTTPResponse::parse_status_line(data.begin(), data.end(), response);
    int header_end = HTTPResponse::parse_header(data.begin() + status_end, data.end(), response);
    int header_end2 = HTTPResponse::parse_header(data.begin() + status_end + header_end, data.end(), response);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("keep-alive", *response->headers["Connection"]);
    ASSERT_EQ(-1, header_end2);
}

TEST(HTTP_Response_Progressive_Parser, ParseResponse) {
    HttpResponseParser parser;

    auto response = parser.get_response();

    const char* buffer1 = "HTTP/1.1 200 OK\r\n";
    ASSERT_TRUE(parser.partial_parse(buffer1, strlen(buffer1)));

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("HTTP/1.1", response->version);

    const char* buffer2 = "Content-Type: ";
    ASSERT_TRUE(parser.partial_parse(buffer2, strlen(buffer2)));

    ASSERT_EQ(response->headers.end(), response->headers.find("Content-Type"));

    const char* buffer3 = "text/plain";
    ASSERT_TRUE(parser.partial_parse(buffer3, strlen(buffer3)));

    ASSERT_EQ(nullptr, response->headers["Content-Type"]);

    const char* buffer4 = "\r\n";
    ASSERT_TRUE(parser.partial_parse(buffer4, strlen(buffer4)));

    ASSERT_EQ("text/plain", *response->headers["Content-Type"]);

    const char* buffer5 = "Content-Length: 11\r\n\r\n";
    ASSERT_TRUE(parser.partial_parse(buffer5, strlen(buffer5)));

    ASSERT_EQ("11", *response->headers["Content-Length"]);

    const char* buffer6 = "Hello";
    ASSERT_TRUE(parser.partial_parse(buffer6, strlen(buffer6)));

    ASSERT_EQ(nullptr, response->body);

    const char* buffer7 = " world";
    ASSERT_FALSE(parser.partial_parse(buffer7, strlen(buffer7)));

    ASSERT_EQ("Hello world", *response->body);
}