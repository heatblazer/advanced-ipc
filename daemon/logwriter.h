#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <pthread.h>

#include "defs.h"

namespace ipc {

typedef void* (*cb)(void*);

class LogWriter : public Policy<pthread_t>
{
public:
    static void* worker(void* args);
    LogWriter();
    ~LogWriter();
    // typicaly for thread creation but params are unused here
    bool create(int stack_size, void* user_data, int prio, cb proxyb);
    void start();
    void stop();
    bool isRunning();
private:
    bool m_isRunning;
    bool m_runner;

};

} // ipc

#endif // LOGWRITER_H
