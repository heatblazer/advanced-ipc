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
      m_mask(mask),
      m_dataNode({NULL, 0}),
      m_currentMsg({NULL, 0})
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
            return res;
        }
    }

    int ret = msgsnd(m_msgId, m_dataNode.data,
                     m_dataNode.size, IPC_NOWAIT);
    if (ret == -1) {
        if (errno != EAGAIN) {
            return res;
        } else {
            if (msgsnd(m_msgId, m_dataNode.data,
                       m_dataNode.size, 0) == -1) {
                res = false;
            }
        }
    }
    res = true;
    return res;
}

bool msg::tryReceive(msg &sender)
{
    bool res = false;
    m_msgId  = msgget(sender.getKey(), m_mask);
    if (m_msgId == -1) {
        // noone create it
        if ((m_msgId= msgget(sender.getKey(), m_mask | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error receiving message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));
            return res;
        }
    }

    if (msgrcv(sender.m_msgId, m_dataNode.data,
               sender.m_dataNode.size + (sizeof(sender)), 0, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            printf("ERRNO: (%d)/(%s)\n", errno, strerror(errno));
            return res;
        }

        if (msgrcv(m_msgId, m_currentMsg.data,
                   m_dataNode.size, 0, 0) == -1) {
            printf("ERRNO: (%d)/(%s)\n", errno, strerror(errno));
            return res;
        }

    }
    res = true;
    printf("Test: From key: (%d) - data: (%s)\n", sender.getKey(),
           (char*)m_dataNode.data);
    return res;
}

void msg::setKey(int newkey)
{
    m_key = newkey;
}

void msg::setData(void *data, int size)
{
    m_dataNode.data = data;
    m_dataNode.size = size;
}

msg::node_t msg::getData()
{
    return m_dataNode;
}

}
