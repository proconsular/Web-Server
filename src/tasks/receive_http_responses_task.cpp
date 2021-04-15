//
// Created by Chris Luttio on 4/1/21.
//

#include "receive_http_responses_task.h"
#include "http_response_parser.h"
#include "constants.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

void ReceiveHTTPResponsesTask::perform() {
    for (const auto& pair: _state->outbound_http_request_queue) {
        auto carrier = std::make_shared<HTTPRequestCarrier>(*pair.second);
        if (carrier->status == SENT) {

            std::shared_ptr<HTTPResponse> response = parse(carrier);

            carrier->status = FULFILLED;

            carrier->http_response = response;
            _controller->apply(Action(ReportLog, response->generate()));
            _controller->apply(Action(ReceiveHttpResponse, carrier));
        }
    }
}

std::shared_ptr<HTTPResponse> ReceiveHTTPResponsesTask::parse(const std::shared_ptr<HTTPRequestCarrier> &carrier) const {
    const ssize_t buffer_size = 100 * KB;

    int socket = carrier->connection->socket.id();

    ssize_t amount = 0;

    HttpResponseParser parser;

    do {
        char* buffer = new char[buffer_size];

        amount = read(socket, buffer, buffer_size);

        if (amount == -1)
            break;

        bool ok = parser.partial_parse(buffer, amount);

        delete[] buffer;

        if (!ok)
            break;
    } while (amount > 0);

    return parser.get_response();
}
