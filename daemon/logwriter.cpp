#include "logwriter.h"

#include <stdio.h>
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
            // do task here
            // lw->log() for ex.
            char buff[256]={0};
            sprintf(buff, "Running a task...\n");
            write(fd, buff, sizeof(buff)/sizeof(buff[0]));
        }
    } while (lw->m_isRunning);

    close(fd);
}

LogWriter::LogWriter()
    : m_isRunning(false),
      m_runner(false)
{

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


}
