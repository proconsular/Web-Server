//
// Created by Chris Luttio on 4/3/21.
//

#ifndef P8_WEB_SERVER_LOG_EVENTS_TASK_H
#define P8_WEB_SERVER_LOG_EVENTS_TASK_H

#include "task.h"
#include "state.h"

class LogEventsTask: public Task {
public:
    explicit LogEventsTask(State* state, std::string );

    void perform() override;
    bool alive() override {
        return _alive;
    }
private:
    State* _state;
    bool _alive;

    int _index;
    std::string _filename;
    FILE *_file;
};


#endif //P8_WEB_SERVER_LOG_EVENTS_TASK_H
