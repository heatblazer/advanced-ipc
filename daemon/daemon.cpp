#include "daemon.h"
#include "msg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // exit() and atoi()
#include <stdbool.h> // true/false

#include <sys/stat.h> // chmod
#include <sys/mount.h> // mout fs
#include <unistd.h> // cwd
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// err //
#include <errno.h>


namespace ipc {

int Daemon::dummy(void *data)
{
    msg* message = (msg*) data;
    int key, mask, msgid;
    message->a = 10000;
    message->b = 20000;
    message->c = 30000;

    key = 1234;
    mask = 0666;
    msgid = msgget(key, mask);

    if (msgid == -1) {
        if ((msgid = msgget(key, mask | IPC_CREAT)) == -1) {
            exit(1);
        }
    }

    for(;;) {
        char buff[sizeof(msg)] = {0};
        memcpy(buff, message, sizeof(msg));
        int ret = msgsnd(msgid, buff, sizeof(buff), IPC_NOWAIT);
        if (ret == -1) {
            if (errno != EAGAIN) {
                exit(1);
            }
            usleep(50000);
            if (msgsnd(msgid, buff, sizeof(buff), 0) == -1) {
                exit(1);
            }
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
    m_data = user_data;
    m_entry = cb;

}

void Daemon::start()
{
    umask(0);

    if ((m_pid = fork()) < 0) {
        exit(FORK_FAILED);
    } else if (m_pid != 0) {
        exit(PARENT);
    } else {
        setsid();

        m_sig.sa_handler = SIG_IGN;
        sigemptyset(&m_sig.sa_mask);
        m_sig.sa_flags = 0;
        
        if(sigaction(SIGHUP, &m_sig, NULL) < 0) {
            fprintf(stderr, "Can`t ignro SIGHUP\n");
            exit(3);
        }

        char pwd[512]={0};
        getcwd(pwd, sizeof(pwd)/sizeof(pwd[0]));

        if (chdir(pwd) < 0) {
            exit(FORK_FAILED);
        }

    }

    if (m_entry && m_data) {
        int res = m_entry(m_data); // user cb must be for(;;)
        // handle res after entry finishes
        (void) res;
    }


}

}   // ipc
