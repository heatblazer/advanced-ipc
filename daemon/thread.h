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

class BSem
{
public:
    BSem();
    ~BSem();
    void init();
private:
    Mutex m_mutex;
    pthread_cond_t m_cond;
    int val;
};

class Thread : public Policy<pthread_t>
{
public:
    static void* worker(void* args);
    static pthread_t currentThread();
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
    Mutex m_mutex;

};

} // ipc

#endif // THREAD_H
