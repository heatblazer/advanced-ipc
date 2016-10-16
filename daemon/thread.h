#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#include "message.h"
#include "defs.h"

namespace ipc {

typedef void* (*cb)(void*);

class Mutex : public Policy<pthread_mutex_t>
{
public:
    Mutex();
    ~Mutex();
    void init();
    void lock();
    void unlock();
};

class Thread : public Policy<pthread_t>
{
public:
    static void* worker(void* args);
    Thread();
    ~Thread();
    // typicaly for thread creation but params are unused here
    bool create(int stack_size, void* user_data, int prio, cb proxyb);
    void stop();
    void join();
    bool isRunning();

private:
    bool m_isRunning;
    bool m_runner;
    pthread_mutex_t m_mutex;

};

} // ipc

#endif // THREAD_H
