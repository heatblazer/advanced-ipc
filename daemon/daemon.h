#ifndef DAEMON_H
#define DAEMON_H

// unix api //
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

// stl //
#include <vector>

// tool for logging //
#include "thread.h"

#include "../message/message.h"

namespace ipc {
class msg;

typedef int (*userCb)(void*);

class Daemon
{
public:
    static void* job(void* args);
    static int dummy(void* data);
    enum ExitCodes {PARENT = 0, CHILD, FORK_FAILED};

    explicit Daemon();
    ~Daemon();

    int create(void* user_data, userCb cb);
    void start(void);

private:
    void error(const char* msg);

private:
    void *m_data;
    pid_t m_pid;
    userCb m_entry;
    struct sigaction m_sig;
    Thread      m_logger;
    Mutex       m_mutex;
    msg         m_daemonMessage;
    std::vector<msg> m_messages;

};

}   // namespace ipc
#endif // DAEMON_H
