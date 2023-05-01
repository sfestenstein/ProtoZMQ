#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

#include <thread>
#include <memory>
#include <functional>
#include <map>
#include <list>

#include <zmq.hpp>

#include <AllZmqMessages.h>


class ZmqSubscriber
{
public:
    using zmqCallbackFunction = std::function<void(void*, size_t)>;
    ZmqSubscriber(std::string proxyConnectionString);
    void subscribeToTopic(const AllZmqMessages::MessageEnums messageType, zmqCallbackFunction function);
private:
    void receiveThread();

    std::thread zmqReceiveThread;
    bool m_running = true;
    std::unique_ptr<zmq::context_t> m_context=nullptr;
    std::unique_ptr<zmq::socket_t> m_socket=nullptr;
    std::string m_connectionString;
    std::map<std::string, std::list<zmqCallbackFunction>> m_registeredFunctions;
};

#endif // ZMQSUBSCRIBER_H
