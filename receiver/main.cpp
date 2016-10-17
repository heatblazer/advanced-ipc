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

// this is a test project
#include "message.h"

int main(int argc, char *argv[])
{

    char data1[] = "m1";
    char data2[] = "m2";

    ipc::msg m1(1234, 8910, 0666);
    m1.setData(data1, sizeof(data1));

    ipc::msg m2(1234, 4567, 0666);
    m2.setData(data2, sizeof(data2));
    m2.setDestination(8910);


    // m1.send(4567, 0);
    m1.setName("ilianz");
    m1.send(1234, 0);

    m2.setName("vladi");
    m2.send(1234, 0);

    return 0;
}
