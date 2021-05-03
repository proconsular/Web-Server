//
// Created by Chris Luttio on 4/17/21.
//

#include "gtest/gtest.h"

#include "applications/docker_interface_app.h"
#include "applications/http_client_app.h"

#include "json.hpp"
#include "utils.h"

#include <thread>

using json = nlohmann::json;

const std::string port = "3000";

TEST(HttpServerTests, Basic_Helloworld) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = HttpMessage::make_request("GET", "/hello.txt");
    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("hello world", *response->body);
}

TEST(HttpServerTests, Basic_Not_Found) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = HttpMessage::make_request("GET", "/random");
    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(404, response->code);
    ASSERT_EQ("Not Found", response->status);

    ASSERT_EQ("", *response->body);
}

TEST(HttpServerTests, Basic_Not_Supported) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = HttpMessage::make_request("POST", "/");
    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(501, response->code);
    ASSERT_EQ("Not Implemented", response->status);
}

TEST(HttpServerTests, Basic_Bad_Request) {
    FILE *big_file = fopen("../../web/big_file.txt", "r");
    char *buffer = new char[150000];
    size_t amount = fread(buffer, 1, 150000, big_file);
    fclose(big_file);

    auto data = std::make_shared<std::string>(buffer, buffer + amount);
    delete[] buffer;

    ASSERT_EQ("Fusce feugiat eget odio ac mollis. Morbi vitae faucibus elit.", data->substr(0, 61));

    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = HttpMessage::make_request("GET", "/big_file.txt");
    request->body = data;
    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);
    ASSERT_EQ(400, response->code);
    ASSERT_EQ("Bad Request", response->status);
}

TEST(HttpServerTests, Reliability_Request) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));

    const int tries = 50;

    int passed = 0;
    int i = 0;
    while (i < tries) {
        json container_info;
        bool ok1 = docker.inspect_container(id, container_info);
        bool ok2 = id == container_info["Id"];
        if (ok1 && ok2)
            passed++;
        i++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(tries, passed);
}

TEST(HttpServerTests, Reliability_Server_OK) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    const int tries = 100;
    int passes = 0;
    int i = 0;

    while (i < tries) {
        HttpClientApp http;
        auto request = HttpMessage::make_request("GET", "/hello.txt");
        std::shared_ptr<HttpMessage> response;
        bool received = http.send(host, request, response);
        bool message_integrity = response->status == "OK" && response->code == 200 && *response->body == "hello world";
        if (received && message_integrity)
            passes++;
        i++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(tries, passes);
}

TEST(HttpServerTests, Reliability_Server_90KB_File) {
    FILE *big_file = fopen("../../web/big_file.txt", "r");
    char *buffer = new char[150000];
    size_t amount = fread(buffer, 1, 150000, big_file);
    fclose(big_file);

    std::string data = std::string(buffer, buffer + amount);
    delete[] buffer;

    ASSERT_EQ("Fusce feugiat eget odio ac mollis. Morbi vitae faucibus elit.", data.substr(0, 61));

    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = HttpMessage::make_request("GET", "/big_file.txt");
    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);

    ASSERT_FALSE(response->body == nullptr);
    ASSERT_EQ(data, *response->body);
}

TEST(HttpServerTests, Reliability_Server_Rounds_90KB_File) {
    FILE *big_file = fopen("../../web/big_file.txt", "r");
    char *buffer = new char[150000];
    size_t amount = fread(buffer, 1, 150000, big_file);
    fclose(big_file);

    std::string data = std::string(buffer, buffer + amount);
    delete[] buffer;

    ASSERT_EQ("Fusce feugiat eget odio ac mollis. Morbi vitae faucibus elit.", data.substr(0, 61));

    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    const int rounds = 100;
    int passes = 0;
    int round = 0;

    while (round < rounds) {
        HttpClientApp http;
        auto request = HttpMessage::make_request("GET", "/big_file.txt");
        std::shared_ptr<HttpMessage> response;
        bool received = http.send(host, request, response);
        if (received && response->body != nullptr) {
            if (data == *response->body) {
                passes++;
            }
        }
        round++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(rounds, passes);
}

TEST(HttpServerTests, Performance_Basic_OK) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    const int tries = 100;
    int passes = 0;
    int i = 0;

    uint32_t highest_duration = 0;

    while (i < tries) {
        HttpClientApp http;
        auto request = HttpMessage::make_request("GET", "/hello.txt");
        std::shared_ptr<HttpMessage> response;
        auto start = get_time();
        bool received = http.send(host, request, response);
        auto end = get_time();
        if (received) {
            bool message_integrity = response->status == "OK" && response->code == 200;
            bool message = *response->body == "hello world";
            if (message_integrity && message) {
                passes++;
                if (get_millisecond_duration(end, start) > highest_duration) {
                    highest_duration = get_millisecond_duration(end, start);
                }
            }
        }
        i++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(tries, passes);
    ASSERT_GE(30, highest_duration);
}

TEST(HttpServerTests, Func_Connection_Header) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;

    int rounds = 3;
    int i = 0;

    while (i++ < rounds) {
        auto request = HttpMessage::make_request("GET", "/hello.txt");
        request->headers["Connection"] = std::make_shared<std::string>("keep-alive");

        std::shared_ptr<HttpMessage> response;
        bool received = http.send(host, request, response);

        ASSERT_TRUE(received);
        ASSERT_EQ(200, response->code);
        ASSERT_EQ("hello world", *response->body);
    }

    ASSERT_EQ(1, http.state()->connections.size());

    docker.remove_container_forced(id);
}

TEST(HttpServerTests, Func_Resend_Close_Connection_Header) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;

    auto request = HttpMessage::make_request("GET", "/hello.txt");
    request->headers["Connection"] = std::make_shared<std::string>("close");

    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    ASSERT_TRUE(received);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("hello world", *response->body);

    auto request2 = HttpMessage::make_request("GET", "/hello.txt");
    request2->headers["Connection"] = std::make_shared<std::string>("close");

    std::shared_ptr<HttpMessage> response2;
    bool received2 = http.send(host, request2, response2);

    ASSERT_TRUE(received2);

    docker.remove_container_forced(id);
}

TEST(HttpServerTests, Func_Close_Connection_Header) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server_test", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    http.state()->config->auto_resend_on_request_failure = false;

    auto request = HttpMessage::make_request("GET", "/hello.txt");
    request->headers["Connection"] = std::make_shared<std::string>("close");

    std::shared_ptr<HttpMessage> response;
    bool received = http.send(host, request, response);

    ASSERT_TRUE(received);
    ASSERT_EQ(200, response->code);
    ASSERT_EQ("hello world", *response->body);

    auto request2 = HttpMessage::make_request("GET", "/hello.txt");
    request2->headers["Connection"] = std::make_shared<std::string>("close");

    std::shared_ptr<HttpMessage> response2;
    bool received2 = http.send(host, request2, response2);

    ASSERT_FALSE(received2);

    docker.remove_container_forced(id);
}