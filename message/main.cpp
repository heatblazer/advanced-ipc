#include "message.h"

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    char data1[] = "m1";
    char data2[] = "m2";

    ipc::msg m1(1234, 0666);
    m1.setData(data1, sizeof(data1));

    ipc::msg m2(4567, 0666);
    m2.setData(data2, sizeof(data2));

    if (m1.trySend(m2)) {
        m2.tryReceive(m1);
    }


    if (m2.trySend(m1)) {
        m1.tryReceive(m2);
    }

}
