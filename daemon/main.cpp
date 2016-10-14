#include "daemon.h"

int main(int argc, char *argv[])
{
    ipc::Daemon d;
//    d.create(&message, ipc::Daemon::dummy);
    d.start();
    return 0;
}
