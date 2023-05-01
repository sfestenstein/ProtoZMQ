#include "../inc/ZmqPublisher.h"

#include <MessageHeartbeat.pb.h> // Get rid of this!

ZmqPublisher::ZmqPublisher(std::string proxyConnectionString) :
    m_socket(m_context, zmq::socket_type::pub)
{
    //Connect to the proxy address.
    m_socket.connect(proxyConnectionString.c_str());
}

void ZmqPublisher::send(const google::protobuf::Message &message,
                        AllZmqMessages::MessageEnums messageType)
{
    auto topicSv = AllZmqMessages::EnumStrings[static_cast<size_t>(messageType)];
    std::string stringifiedMessage;
    message.SerializeToString(&stringifiedMessage);

    zmq::message_t topicMsg(topicSv.length());
    std::memcpy(topicMsg.data(), topicSv.data(), topicSv.length());

    m_socket.send(topicMsg, zmq::send_flags::sndmore);
    m_socket.send(zmq::buffer(stringifiedMessage.c_str(),
                              stringifiedMessage.length()));
 }
