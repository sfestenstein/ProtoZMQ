#include <ZmqPublisher.h>

ZmqPublisher::ZmqPublisher(std::string proxyConnectionString) :
    m_socket(m_context, zmq::socket_type::pub)
{
    //Connect to the proxy address.
    m_socket.connect(proxyConnectionString.c_str());
}

ZmqPublisher::~ZmqPublisher()
{
    m_socket.close();
}

void ZmqPublisher::send(const google::protobuf::Message &message,
                        AllZmqMessages::MessageEnums messageType)
{
    auto &topicMsg = AllZmqMessages::TopicMessages[static_cast<size_t>(messageType)];

    // Design Consideration! we are stringifying the message, then probably
    // doing a memcpy when creating and sending the zmq::buffer.  There has
    // to be a better way!
    std::string stringifiedMessage;
    message.SerializeToString(&stringifiedMessage);

    m_socket.send(topicMsg, zmq::send_flags::sndmore);
    m_socket.send(zmq::buffer(stringifiedMessage.c_str(),
                              stringifiedMessage.length()));
 }
