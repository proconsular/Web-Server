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
    if (http.send("https://banned.video", request, response)) {
        std::cout << *response->generate() << std::endl;
//        FILE* page = fopen("web/test.html", "w");
//        int written = 0;
//        int cursor = 0;
//        do {
//            auto str = response->body->substr(cursor, 1024 * 10);
//            written = fwrite(str.c_str(), 1, 1024 * 10, page);
//            if (written >= 0)
//                cursor += written;
//        } while (cursor < response->body->size());
    }

    return 0;
}