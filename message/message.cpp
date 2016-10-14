#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>


namespace ipc {

msg::msg(int key, int mask)
    : m_key(key),
      m_msgId(-1),
      m_mask(mask)
{

}

msg::~msg()
{

}

int msg::getKey() const
{
    return m_key;
}

bool msg::trySend(msg &receiver)
{
    (void) receiver;
    bool res = false;
    m_msgId = msgget(receiver.getKey(), m_mask);
    if (m_msgId == -1) {
        // noone create it
        if ((m_msgId = msgget(receiver.getKey(), m_mask | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error creating message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));
            return false;
        }
    }
    char test[] = "hello test message!";
    int ret = msgsnd(m_msgId, test, sizeof(test), IPC_NOWAIT);
    if (ret == -1) {
        if (errno != EAGAIN) {
            return res;
        } else {
            if (msgsnd(m_msgId, test, sizeof(test), 0) == -1) {
                res = false;
            }
        }
    }

    return res;
}

bool msg::tryReceive(msg &sender)
{
    bool res = false;
    m_msgId = msgget(sender.getKey(), m_mask);
    if (m_msgId == -1) {
        // noone create it
        if ((m_msgId = msgget(sender.getKey(), m_mask | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error creating message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));
            return res;
        }
    }

    char buff[512]={0};
    if (msgrcv(m_msgId, buff, sizeof(buff), 0, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            return res;
        }

        if (msgrcv(m_msgId, buff, sizeof(buff), 0, 0) == -1) {
            return res;
        }

    }
    res = true;
    printf("Got message from:[%d] : (%s)\n",
           sender.getKey(), buff);


    return res;
}

void msg::setKey(int newkey)
{
    m_key = newkey;
}

}
