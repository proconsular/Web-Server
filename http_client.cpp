//
// Created by Chris Luttio on 4/16/21.
//

#include <unistd.h>
#include "applications/http_client_app.h"
#include "json.hpp"
#include "utils.h"

#include "applications/docker_interface_app.h"

using json = nlohmann::json;

int main() {
//    HttpClientApp client;
//
//    json body = {
//            {"Image", "p8_web_server"}
//    };
//
//    auto request = std::make_shared<HTTPRequest>("POST", "/containers/create");
//    request->headers["Content-Type"] = "application/json";
//    request->body = std::make_shared<std::string>(body.dump());
//
//    auto response = client.send("unix:///var/run/docker.sock", request);
//
//    std::cout << response->code << "\n" << *response->body << std::endl;

    HttpClientApp http;

    auto request = HttpMessage::make_request("GET", "/");

    std::shared_ptr<HttpMessage> response;
    if (http.send("http://www.google.com", request, response)) {
        std::cout << *response->generate() << std::endl;
    }

    return 0;
}