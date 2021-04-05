//
// Created by Chris Luttio on 4/3/21.
//

#include "log_events_task.h"

#include <utility>

LogEventsTask::LogEventsTask(State *state, std::string filename): _state(state), _alive(true), _index(0), _filename(std::move(filename)) {
    _file = fopen(_filename.c_str(), "w");
}

void LogEventsTask::perform() {
//    std::map<Events, std::string> table = {
//            {ServerInitialized, "INITIALIZED Server"},
//            {ConfigurationLoaded, "LOADED Configuration"},
//            {CreateClientConnection, "ESTABLISHED Client Connection"},
//            {ClientConnectionExpired, "EXPIRED Client Connection"},
//            {ReceivedHttpRequest, "RECEIVED HTTP Request"},
//            {ProcessedHttpRequest, "Processed Http Request"},
//            {CreatedClientRequest, "CREATED Client Request"},
//            {CompletedClientRequest, "COMPLETED Client Request"},
//    };
//    for (; _index < _state->events.size(); _index++) {
//        Events event = _state->events[_index];
//        auto name = table[event];
//        std::cout << "INFO: " << name << std::endl;
////        fwrite(name.c_str(), sizeof(char), name.size(), _file);
//    }
}