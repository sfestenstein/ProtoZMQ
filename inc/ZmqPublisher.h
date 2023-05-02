#ifndef ZMQPUBLISHER_H
#define ZMQPUBLISHER_H

#include <zmq.hpp>
#include <google/protobuf/message.h>

#include <AllZmqMessages.h>


class ZmqPublisher
{
public:
    ZmqPublisher(std::string proxyConnectionString);
    void send(const google::protobuf::Message &message,
              AllZmqMessages::MessageEnums messageType);
private:
    // TODO! Context should be at a higher scope to avoid
    // duplications.  It will be common for
    static zmq::context_t m_context;
    zmq::socket_t m_socket;
};

#endif // ZMQPUBLISHER_H
