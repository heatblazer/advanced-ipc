#include "thread.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace ipc {


Thread::Thread()
    : m_isRunning(false),
      m_runner(false)
{
}

Thread::~Thread()
{
}

bool Thread::create(int stack_size, void *user_data, int prio, cb proxy)
{
    bool ret = false;
    (void) stack_size;
    m_isRunning = true;
    pthread_mutex_init(&m_mutex, NULL);
    int res = pthread_create(&m_type, NULL, proxy, user_data);
    if (res != 0) {
        return ret;
    }
    ret = true;
    return ret;
}



void Thread::stop()
{
    m_isRunning = false;
}

void Thread::join()
{
    pthread_join(m_type, 0);
}

bool Thread::isRunning()
{
    return m_isRunning;
}

Mutex::Mutex()
{

}

Mutex::~Mutex()
{

}

void Mutex::init()
{
     pthread_mutex_init(&m_type, NULL);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m_type);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_type);
}

} // ipc
