#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>


namespace ipc {

union copy_u {
    msg m;
    char c[sizeof(msg)];
};

msg::msg()
    : m_key(-1),
      m_dest(-1),
      m_msgId(-1),
      m_mask(-1),
      m_serverKey(-1) // key to the router
{

}

msg::msg(int server, int key, int mask)
    : m_key(key),
      m_dest(-1),
      m_msgId(-1),
      m_mask(mask),
      m_serverKey(server)
{

}
msg::msg(const msg &ref)
{
    m_key = ref.m_key;
    m_dest = ref.m_dest;
    m_msgId = ref.m_msgId;
    m_mask = ref.m_mask;
    m_serverKey = ref.m_serverKey;
    strncpy(m_name, ref.m_name, sizeof(m_name)/sizeof(m_name[0]));
}

msg::~msg()
{

}

int msg::getKey() const
{
    return m_key;
}

bool msg::send(int key, void* data)
{
    (void) data;
    bool res = false;
    m_msgId = msgget(key, m_mask);
    if (m_msgId == -1) {
        // noone create it
        if ((m_msgId = msgget(key, m_mask | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error creating message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));
            return res;
        }
    }
    // this is desired to ipc messaging
    struct  {
        long val; // val is important
        char buff[sizeof(msg)];
    } msgbuff ;

    msgbuff.val = sizeof(msg);
    memcpy(msgbuff.buff, this, sizeof(*this));

    int ret = msgsnd(m_msgId, &msgbuff,
                     sizeof(msgbuff), IPC_NOWAIT);
    if (ret == -1) {
        if (errno != EAGAIN) {
            fprintf(stderr, "Error creating message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));

            return res;

        } else {
            if (msgsnd(m_msgId, &msgbuff,
                       sizeof(msgbuff), 0) == -1) {
                fprintf(stderr, "Error creating message: %d:(%s)\n",
                                                errno,
                                                strerror(errno));
                return res;
            }
        }
    }

    res = true;
    return res;
}

msg msg::receive(int key)
{
    msg ret(key, m_key, 0666);
    m_msgId  = msgget(key, m_mask);
    if (m_msgId == -1) {
        // noone create it
        if ((m_msgId= msgget(key, m_mask | IPC_CREAT)) == -1) {
            fprintf(stderr, "Error receiving message: %d:(%s)\n",
                                            errno,
                                            strerror(errno));
            return ret;
        }
    }

    struct {
        long val;
        char buff[sizeof(msg)];
    } msgbuff;

    msgbuff.val = sizeof(msg);
    memcpy(msgbuff.buff, this, sizeof(msg));

    if (msgrcv(m_msgId, &msgbuff,
               sizeof(msgbuff), 0, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            printf("ERRNO: (%d)/(%s)\n", errno, strerror(errno));
            return ret;
        }

        if (msgrcv(m_msgId, &msgbuff,
                   sizeof(msgbuff), 0, 0) == -1) {
            printf("ERRNO: (%d)/(%s)\n", errno, strerror(errno));
            return ret;
        }

    }

    // test extraction
    msg* m = (msg*) msgbuff.buff;
    ret = *m;
    return ret;
}


void msg::setKey(int newkey)
{
    m_key = newkey;
}

void msg::setData(void *data, int size)
{
    (void) data;
    (void) size;
}

void msg::setDestination(int dest)
{
    m_dest = dest;
}

void msg::setName(const char *n)
{
    strncpy(m_name, n, sizeof(m_name)/sizeof(m_name[0]));
}

char *msg::getMessage()
{
    static char msg[256] = {0};
    sprintf(msg, "class msg:\n"
           "name: [%s]\n"
           "key: [%d]\n"
           "id: [%d]\n"
           "send to: [%d]\n"
           "mask: [%d]\n"
           "server: [%d]\n",
           m_name,
           m_key, m_msgId, m_dest,
           m_mask,
           m_serverKey);

    return msg;
}


}
