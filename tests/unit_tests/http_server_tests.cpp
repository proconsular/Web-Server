//
// Created by Chris Luttio on 4/17/21.
//

#include "gtest/gtest.h"

#include "applications/docker_interface_app.h"
#include "applications/http_client_app.h"

#include "json.hpp"
#include "utils.h"

using json = nlohmann::json;

const std::string port = "3003";

TEST(HttpServerTests, helloworld) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = std::make_shared<HTTPRequest>("GET", "/hello.txt");
    std::shared_ptr<HTTPResponse> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("hello world", *response->body);
}

TEST(HttpServerTests, NotFound) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = std::make_shared<HTTPRequest>("GET", "/random");
    std::shared_ptr<HTTPResponse> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(404, response->code);
    ASSERT_EQ("Not Found", response->status);
    ASSERT_EQ("", *response->body);
}

TEST(HttpServerTests, HTML) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = std::make_shared<HTTPRequest>("GET", "/");
    std::shared_ptr<HTTPResponse> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(200, response->code);
    ASSERT_EQ("OK", response->status);
    ASSERT_EQ("text/html", *response->headers["Content-Type"]);
    ASSERT_EQ("<html>\n"
              "<head>\n"
              "    <title>Hello World</title>\n"
              "    <link rel=\"stylesheet\" href=\"style.css\" />\n"
              "</head>", response->body->substr(0, 99));
}

TEST(HttpServerTests, NotSupported) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    HttpClientApp http;
    auto request = std::make_shared<HTTPRequest>("POST", "/");
    std::shared_ptr<HTTPResponse> response;
    bool received = http.send(host, request, response);

    docker.remove_container_forced(id);

    ASSERT_TRUE(received);

    ASSERT_EQ(501, response->code);
    ASSERT_EQ("Not Implemented", response->status);
}

TEST(HttpServerTests, Reliability_Request) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
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
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    const int tries = 100;
    int passes = 0;
    int i = 0;

    while (i < tries) {
        HttpClientApp http;
        auto request = std::make_shared<HTTPRequest>("GET", "/hello.txt");
        std::shared_ptr<HTTPResponse> response;
        bool received = http.send(host, request, response);
        bool message_integrity = response->status == "OK" && response->code == 200 && *response->body == "hello world";
        if (received && message_integrity)
            passes++;
        i++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(tries, passes);
}

TEST(HttpServerTests, Reliability_Server_HTML) {
    DockerInterfaceApp docker;

    const std::string host = string_format("http://localhost:%s", port.c_str());

    std::string id;
    ASSERT_TRUE(docker.create_container(id, "p8_web_server", "8001", port));
    ASSERT_TRUE(docker.start_container(id));
    ASSERT_TRUE(docker.wait_for_container_status(id, "running"));

    const int tries = 100;
    int passes = 0;
    int i = 0;

    while (i < tries) {
        HttpClientApp http;
        auto request = std::make_shared<HTTPRequest>("GET", "/");
        std::shared_ptr<HTTPResponse> response;
        bool received = http.send(host, request, response);
        bool message_integrity = response->status == "OK" && response->code == 200;
        bool message = response->body->substr(0, 99) == "<html>\n"
                                          "<head>\n"
                                          "    <title>Hello World</title>\n"
                                          "    <link rel=\"stylesheet\" href=\"style.css\" />\n"
                                          "</head>";
        if (received && message_integrity && message)
            passes++;
        i++;
    }

    docker.remove_container_forced(id);

    ASSERT_EQ(tries, passes);
}