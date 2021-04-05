//
// Created by Chris Luttio on 3/23/21.
//

#include "gtest/gtest.h"
#include "http_request_parser.h"
#include "utils.h"

TEST(HTTPRequestParserTests, RequestLine) {
    std::string data = "GET / HTTP/1.1";
    HTTPRequest* request = HTTPRequestParser::parse(data);
    ASSERT_EQ("GET", request->method);
    ASSERT_EQ("/", request->uri.to_string());
    ASSERT_EQ("HTTP/1.1", request->version);
}

TEST(HTTPRequestParserTests, Split) {
    std::string test = "hello,,world";
    auto output = split_string(",", test);
    ASSERT_EQ("hello", output[0]);
    ASSERT_EQ("", output[1]);
    ASSERT_EQ("world", output[2]);

    auto output2 = split_string("\r\n", "hello\r\nworld\rdog");
    ASSERT_EQ("hello", output2[0]);
    ASSERT_EQ("world\rdog", output2[1]);
}

TEST(HTTPRequestParserTests, RequestLine_Headers) {
    std::string data = "GET / HTTP/1.1\r\nHost: helloworld\r\ncontent-type:plain/text";
    HTTPRequest* request = HTTPRequestParser::parse(data);
    ASSERT_EQ("GET", request->method);
    ASSERT_EQ("/", request->uri.to_string());
    ASSERT_EQ("HTTP/1.1", request->version);
    ASSERT_EQ("helloworld", request->headers["Host"]);
    ASSERT_EQ("plain/text", request->headers["content-type"]);
}

TEST(HTTPRequestParserTests, RequestLine_Headers_Body) {
    std::string data = "GET / HTTP/1.1\r\nContent-Length: 11\r\n\r\nHello world";
    HTTPRequest* request = HTTPRequestParser::parse(data);
    ASSERT_EQ("GET", request->method);
    ASSERT_EQ("/", request->uri.to_string());
    ASSERT_EQ("HTTP/1.1", request->version);
    ASSERT_EQ("11", request->headers["Content-Length"]);
    ASSERT_EQ("Hello world", request->body);
}