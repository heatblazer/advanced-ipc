#ifndef MESSAGE_H
#define MESSAGE_H

// this class specifies the message
// that is send and recieved trough processes
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
    virtual ~msg();

    virtual bool trySend(msg& receiver);
    virtual bool tryReceive(msg& sender);

    // these functions will be sending keys via
    // processes
    bool send(int key, void *data);
    bool receive(int key);

    int getKey() const;
    void setKey(int newkey);
    void setData(void* data, int size);
    node_t getData();

    void print();
private:
    // we must be able to share the
    // key and retrieve it to whoever got
    //it
    int m_key;
    int m_msgId;
    int m_mask;

    int m_serverKey;
    node_t m_dataNode;
    node_t m_currentMsg;

};
}


#endif // MESSAGE_H
