#include "logwriter.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace ipc {

void *LogWriter::worker(void *args)
{
    LogWriter* lw = (LogWriter*) args;

    int fd = open("log.log", O_RDONLY | O_WRONLY | O_CREAT);

    if (fd < 0) {
        return NULL;
    }
    do {
        sleep(1);
        if (lw->m_runner) {
            if (lw->m_buff) {
                pthread_mutex_lock(&lw->m_mutex);
                write(fd, lw->m_buff, sizeof(lw->m_buff));
                memset(lw->m_buff, 0, sizeof(lw->m_buff));
                pthread_mutex_unlock(&lw->m_mutex);
            }
        }
    } while (lw->m_isRunning);

    close(fd);
    return NULL;
}

LogWriter::LogWriter()
    : m_isRunning(false),
      m_runner(false)
{
    memset(m_buff, 0, sizeof(m_buff)/sizeof(m_buff[0]));
}

LogWriter::~LogWriter()
{
    m_isRunning = false;
    pthread_join(m_type, 0);
}

bool LogWriter::create(int stack_size, void *user_data, int prio, cb proxy)
{
    bool ret = false;
    (void) stack_size;
    (void) prio;
    (void) user_data;
    (void) proxy;

    pthread_mutex_init(&m_mutex, NULL);
    m_isRunning = true;
    int res = pthread_create(&m_type, 0, LogWriter::worker, this);
    if (res != 0) {
        return ret;
    }
    ret = true;
    return ret;

}

void LogWriter::start()
{
     m_runner = true;
}

void LogWriter::stop()
{
    m_runner = false;
}

void LogWriter::log(const char *msg)
{
    pthread_mutex_lock(&m_mutex);
    strcpy(m_buff, msg);
    pthread_mutex_unlock(&m_mutex);
}


}
