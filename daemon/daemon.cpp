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

namespace ipc {


void *Daemon::job(void *args)
{
    // here we log messagges
    Daemon* d = (Daemon*) args;

#if 0
    int fd = open("log.log", O_RDONLY | O_WRONLY | O_CREAT);
    if (fd < 0) {
        return NULL;
    }
#endif
    do {
        usleep(100);
   //     char msg[64]={"test message"};
        d->m_mutex.lock();
   //     write(fd, msg, sizeof(msg));
        d->m_mutex.unlock();
    } while (d->m_logger.isRunning());

//    close(fd);

}

int Daemon::dummy(void *data)
{
    Daemon* d =  (Daemon*) data;

    d->m_logger.create(0, d, 0, Daemon::job);

    for(;;) {
        usleep(100);
        // receive anything sent to 1234
        msg m = d->m_daemonMessage.receive(1234);
        if (m.m_dest >= 0) {
            m.send(m.m_dest, NULL);
        }
    }

    return 0;
}

Daemon::Daemon()
    : m_daemonMessage(1234, 1234, 0666)
{
    m_mutex.init();
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
    // call the worker
    if (m_entry && m_data) {
        int res = m_entry(m_data); // user cb must be for(;;)
        // handle res after entry finishes
        (void) res;
    }

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
#if 0
    // call the worker
    if (m_entry && m_data) {
        int res = m_entry(m_data); // user cb must be for(;;)
        // handle res after entry finishes
        (void) res;
    }
#endif
}

void Daemon::error(const char *msg)
{
    (void) msg;
}

}   // ipc
