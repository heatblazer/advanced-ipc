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
    explicit msg(int key, int mask);
    virtual ~msg();

    virtual bool trySend(msg& receiver);
    virtual bool tryReceive(msg& sender);

    int getKey() const;
    void setKey(int newkey);
    void setData(void* data, int size);
    node_t getData();

private:
    int m_key;
    int m_msgId;
    int m_mask;

    node_t m_dataNode;
    node_t m_currentMsg;

};
}


#endif // MESSAGE_H
