#ifndef MESSAGE_H
#define MESSAGE_H

// this class specifies the message
// that is send and recieved trough processes
// every program must know about messages
namespace ipc {

/// the msg may be further
/// extended for more
/// advanced messages
/// \brief The msg class
///
class msg
{
    struct node_t {
        void* data;
        int size;
    } ;
public:
    explicit msg(int server, int key, int mask);
    msg(const msg& ref);
    ~msg(); // do we need to extend it? not for now

    // these functions will be sending keys via
    // processes
    bool send(int key, void *data);
    bool receive(int key);

    int getKey() const;
    void setKey(int newkey);
    void setData(void* data, int size);
    void setDestination(int dest);

    node_t getData();
    void print();

private:
    // we must be able to share the
    // key and retrieve it to whoever got
    //it
    int m_key;
    int m_dest;
    int m_msgId;
    int m_mask;

    int m_serverKey;
    node_t m_dataNode;
};

}


#endif // MESSAGE_H
