#ifndef MESSAGE_H
#define MESSAGE_H

// this class specifies the message
// that is send and recieved trough processes
// every program must know about messages
namespace ipc {

class msg
{

public:
    msg();
    msg(int server, int key, int mask);
    msg(const msg& ref);
    //~msg(); // do we need to extend it? not for now

    // these functions will be sending keys via
    // processes
    bool send(int key, void *data);
    bool receive(int key);

    int getKey() const;
    void setKey(int newkey);
    void setData(void* data, int size);
    void setDestination(int dest);
    void print();
    void setName(const char* n);

private:
    // we must be able to share the
    // key and retrieve it to whoever got
    //it
    int m_key;
    int m_dest;
    int m_msgId;
    int m_mask;
    int m_serverKey;    
    char m_name[64];
};

}


#endif // MESSAGE_H
