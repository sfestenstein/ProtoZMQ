#ifndef ZMQPUBLISHER_H
#define ZMQPUBLISHER_H

#include <zmq.hpp>
#include <google/protobuf/message.h>

#include <AllZmqMessages.h>
#include <ZmqContext.h>


class ZmqPublisher : public ZmqContext
{
public:
    ZmqPublisher(std::string proxyConnectionString);

    virtual ~ZmqPublisher();
    ZmqPublisher(const ZmqPublisher&) = delete;
    ZmqPublisher& operator=(ZmqPublisher&) = delete;
    ZmqPublisher(const ZmqPublisher&&) = delete;
    ZmqPublisher& operator=(ZmqPublisher&&) = delete;

    void send(const google::protobuf::Message &message,
              AllZmqMessages::MessageEnums messageType);
private:
    zmq::socket_t m_socket;
};

#endif // ZMQPUBLISHER_H
