#include "ZmqSubscriber.h"

#include <MessageHeartbeat.pb.h>

ZmqSubscriber::ZmqSubscriber(std::string proxyConnectionString) :
    m_connectionString(proxyConnectionString)
{
    zmqReceiveThread = std::thread(&ZmqSubscriber::receiveThread, this);
}

void ZmqSubscriber::subscribeToTopic(const AllZmqMessages::MessageEnums messageType, std::function<void(void*, size_t)> function)
{
    std::string topic = AllZmqMessages::GetEnumString(messageType).data();

    m_registeredFunctions[topic].push_back(function);
    m_socket->setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size()-1);
}

void ZmqSubscriber::receiveThread()
{
    m_context = std::unique_ptr<zmq::context_t>(new zmq::context_t());
    m_socket = std::unique_ptr<zmq::socket_t>(new zmq::socket_t(*m_context, ZMQ_SUB));
    m_socket->connect(m_connectionString.c_str());

    while (m_running)
    {
        zmq::message_t topicMessage;
        zmq::message_t gpbMessage;
        if (m_socket->recv(&topicMessage))
        {
            m_socket->recv(&gpbMessage);
            std::string topic = std::string(static_cast<char*>(topicMessage.data()), topicMessage.size());
            auto functionsToCall = m_registeredFunctions.find(topic);
            if (functionsToCall == m_registeredFunctions.end()) continue;

            for (auto functionToCall : (*functionsToCall).second)
            {
                functionToCall(gpbMessage.data(), gpbMessage.size());
            }
        }
    }

}
