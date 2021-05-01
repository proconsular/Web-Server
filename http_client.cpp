//
// Created by Chris Luttio on 4/16/21.
//

#include <unistd.h>
#include "applications/http_client_app.h"
#include "json.hpp"
#include "utils.h"

#include "applications/docker_interface_app.h"

#include <openssl/ssl.h>

using json = nlohmann::json;

int main() {
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();

    HttpClientApp http;
    http.enable_logging("log2.txt");
//    http.state()->config->auto_reconnect_on_request_failure = false;

    auto request = HttpMessage::make_request("GET", "/");

    std::shared_ptr<HttpMessage> response;
    if (http.send("https://drade.io", request, response)) {
        std::cout << *response->generate() << std::endl;
    }

//    int sock = socket()

    return 0;
}