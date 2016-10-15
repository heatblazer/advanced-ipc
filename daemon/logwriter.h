#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <pthread.h>

#include "message.h"
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
    void log(const char* msg);
private:
    bool m_isRunning;
    bool m_runner;
    char m_buff[512];
    pthread_mutex_t m_mutex;

};

} // ipc

#endif // LOGWRITER_H
