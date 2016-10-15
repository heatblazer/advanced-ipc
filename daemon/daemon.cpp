#include "daemon.h"

// K&R //
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit() and atoi()
#include <stdbool.h> // true/false

// unix //
#include <sys/stat.h> // chmod
#include <sys/mount.h> // mout fs
#include <sys/resource.h>
#include <unistd.h> // cwd
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// err //
#include <errno.h>

#include "../message/message.h"

namespace ipc {

int Daemon::dummy(void *data)
{
    Daemon* d =  (Daemon*) data;

    d->m_logger.create(0, NULL, 0, NULL);
    d->m_logger.start();

    for(;;) {
        usleep(100);

        msg m = d->m_daemonMessage.receive(1234);
        if (m.ok()) {
            d->m_messages.push_back(m);
        }
    }

    return 0;
}

Daemon::Daemon()
{

}

Daemon::~Daemon()
{

}

int Daemon::create(void *user_data, userCb cb)
{
    (void) user_data;
    m_data = this;
    m_entry = cb;
    return 0;
}

void Daemon::start()
{
    umask(0);
    struct rlimit rl;

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "Error: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(FORK_FAILED);
    }

    m_pid = fork();

    if (m_pid < 0) {
        fprintf(stderr, "Fork failed: (%d) : (%s)\n", errno,
                strerror(errno));
        exit(3);
    } else  if (m_pid != 0){
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1) {
        exit(EXIT_FAILURE);
    }

    m_sig.sa_handler = SIG_IGN;
    sigemptyset(&m_sig.sa_mask);
    m_sig.sa_flags = 0;

    if(sigaction(SIGHUP, &m_sig, NULL) < 0) {
        fprintf(stderr, "Can`t ignore SIGHUP\n");
        exit(3);
    }

    if ((m_pid = fork()) < 0) {
        exit(FORK_FAILED);
    } else if (m_pid > 0) {
        exit(0);
    }

    char pwd[512]={0};
    getcwd(pwd, sizeof(pwd)/sizeof(pwd[0]));

    if (chdir(pwd) < 0) {
        exit(3);
    }

    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for(unsigned i=0; i < rl.rlim_max; ++i) {
        close(i);
    }

    int fd0, fd1, fd2;
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    // call the worker
    if (m_entry && m_data) {
        int res = m_entry(m_data); // user cb must be for(;;)
        // handle res after entry finishes
        (void) res;
    }
}

void Daemon::error(const char *msg)
{
    (void) msg;
}

}   // ipc
