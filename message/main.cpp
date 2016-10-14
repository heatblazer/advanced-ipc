#include "message.h"

int main(int argc, char** argv)
{
    ipc::msg m1(1234, 0666);
    ipc::msg m2(4567, 0666);
    m1.trySend(m2);
    m2.tryReceive(m1);
    m2.trySend(m1);
    m1.tryReceive(m2);

}
