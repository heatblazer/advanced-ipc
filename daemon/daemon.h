#ifndef DAEMON_H
#define DAEMON_H

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

namespace ipc {

typedef int (*userCb)(void*);

class Daemon
{
public:
    static int dummy(void* data);
    enum ExitCodes {PARENT = 0, CHILD, FORK_FAILED};

    explicit Daemon();
    ~Daemon();

    int create(void* user_data, userCb cb);
    void start(void);


private:
    void *m_data;
    pid_t m_pid;
    userCb m_entry;
    struct sigaction m_sig;
};

}   // namespace ipc
#endif // DAEMON_H
