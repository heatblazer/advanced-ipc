#include "daemon.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    ipc::Daemon d;
    d.create(nullptr, ipc::Daemon::dummy);
    d.start();
    return 0;
}
