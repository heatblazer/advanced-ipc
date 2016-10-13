#include "daemon.h"
#include "msg.h"

int main(int argc, char *argv[])
{
    ipc::msg message;
    ipc::Daemon d;
    d.create(&message, ipc::Daemon::dummy);
    d.start();
    return 0;
}
