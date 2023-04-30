#include "ZmqSubscriber.h"

#include <AllZmqMessage.h>
#include <MessageHeartbeat.pb.h>

ZmqSubscriber::ZmqSubscriber(std::string proxyConnectionString) :
    m_connectionString(proxyConnectionString)
{
    zmqReceiveThread = std::thread(&ZmqSubscriber::receiveThread, this);
}

void ZmqSubscriber::receiveThread()
{
    m_context = std::unique_ptr<zmq::context_t>(new zmq::context_t());
    m_socket = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(*m_context, ZMQ_SUB));
    auto heartbeatTopic = AllZmqMessages::GetEnumString(AllZmqMessages::MessageEnums::MESSAGE_HEARTBEAT);
    m_socket->connect(m_connectionString.c_str());
    m_socket->setsockopt(ZMQ_SUBSCRIBE, heartbeatTopic.data(), heartbeatTopic.size()-1);

    MessageHeartbeat heartbeatMsg;
    char topic[64];
    while (m_running)
    {
        zmq::message_t topicMessage;
        zmq::message_t gpbMessage;
        heartbeatMsg.Clear();
        if (m_socket->recv(&topicMessage, ZMQ_RCVMORE))
        {
            m_socket->recv(&gpbMessage);
            std::memcpy(topic, topicMessage.data(), topicMessage.size());
            topic[topicMessage.size()] = '\0';
            std::cout << "Received topic " << topic << std::endl;
            heartbeatMsg.ParseFromArray(gpbMessage.data(), gpbMessage.size());
            std::cout << "Recieved Heartbeat " <<  gpbMessage.size() << std::endl;
            std::cout << heartbeatMsg.DebugString() << std::endl;
        }
    }

}
