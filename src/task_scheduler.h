//
// Created by Chris Luttio on 3/26/21.
//

#ifndef P8_WEB_SERVER_TASK_SCHEDULER_H
#define P8_WEB_SERVER_TASK_SCHEDULER_H

#include <vector>

#include "tasks/task.h"

class TaskScheduler {
public:
    void run();

    void add(Task*);
private:
    std::vector<Task*> tasks;
};


#endif //P8_WEB_SERVER_TASK_SCHEDULER_H
