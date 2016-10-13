#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class msg
{
public:
    int a;
    int b;
    int c;
    int add_a_b_c(void) { return a+b+c;}
};

int main(int argc, char *argv[])
{
    int key, mask, msgid;

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
        if (msgrcv(msgid, &buff, sizeof(msg), 0, IPC_NOWAIT) == -1) {
            if (errno != ENOMSG) {
                exit(1);
            }
            usleep(50000);
            if (msgrcv(msgid, &buff, sizeof(msg), 0, 0) == -1) {
                exit(1);
            }
            msg* m = (msg*) buff;
            printf("Got a message...from daemon: [%d][%d][%d]\n"
                   "sum: [%d]\n",
                   m->a, m->b, m->c, m->add_a_b_c());
        }
    }

    return 0;
}
