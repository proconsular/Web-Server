//
// Created by Chris Luttio on 4/3/21.
//

#include "gtest/gtest.h"
#include "http_response.h"
#include "utils.h"

TEST(HTTP_Response_Progressive_Parser, ParseHeader1) {
    const std::string data =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 11\r\n\r\n"
            "Hello World";
    HTTPResponse* response = new HTTPResponse;
    int header_end = HTTPResponse::parse_header(data.begin(), data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", response->headers["Content-Length"]);
}

TEST(HTTP_Response_Progressive_Parser, BadRequest) {
    const std::string data =
            "HTTP/1.1 400 Bad Request\r\nContent-Length: 11\r\n\r\nHello World";
    HTTPResponse* response = new HTTPResponse;
    int header_end = HTTPResponse::parse_header(data.begin(), data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(400, response->code);
    ASSERT_EQ("Bad Request", response->status);
    ASSERT_EQ("11", response->headers["Content-Length"]);
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
    HTTPResponse* response = new HTTPResponse;
    int header_end = HTTPResponse::parse_header(data.begin(), data.end(), response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", response->headers["Content-Length"]);
    ASSERT_EQ("123", response->headers["Content-Encoding"]);
    ASSERT_EQ("World", response->headers["Hello"]);
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
    auto * response = new HTTPResponse;
    int header_end = HTTPResponse::parse_header(data.begin(), data.end(), response);
    HTTPResponse::parse_body(data.begin() + header_end, response);
    ASSERT_EQ(data.size() - 11, header_end);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("11", response->headers["Content-Length"]);
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
    const std::string data =
            "HTTP/1.1 200 OK\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "b\r\n"
            "Hello World\r\n"
            "0\r\n";
    auto * response = new HTTPResponse;
    int header_end = HTTPResponse::parse_header(data.begin(), data.end(), response);
    int size1;
    auto chunk1 = HTTPResponse::parse_chunk_header(data, header_end, size1);
    HTTPResponse::parse_chunk(data, chunk1, size1, response);
    ASSERT_EQ("HTTP/1.1", response->version);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ(11, size1);
    ASSERT_EQ(header_end + 3, chunk1);
    ASSERT_EQ("Hello World", *response->body);
}