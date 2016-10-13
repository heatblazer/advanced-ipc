#ifndef MSG_H
#define MSG_H

#include <stdio.h>

namespace ipc {

// test message
class msg
{
public:
    int a;
    int b;
    int c;
    int add_a_b_c(void);
};

}

#endif // MSG_H
