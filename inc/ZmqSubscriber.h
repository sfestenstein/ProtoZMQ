#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

#include <thread>
#include <memory>
#include <functional>
#include <map>
#include <list>
#include <condition_variable>
#include <mutex>

#include <zmq.hpp>

#include <AllZmqMessages.h>


class ZmqSubscriber
{
public:
    using zmqCallbackFunction = std::function<void(void*, size_t)>;
    ZmqSubscriber(std::string proxyConnectionString);

    // Future Work: subscribe using a weak pointer to a callback function?
    //              This will make unsubscribing very easy.
    void subscribeToTopic(const AllZmqMessages::MessageEnums messageType, 
                          zmqCallbackFunction function);
private:
    void receiveThread();
    void processingThread();
    void processMessagesOffList();

    std::thread m_receiveThread;
    std::thread m_processingThread;
    bool m_running = true;
    std::unique_ptr<zmq::context_t> m_context=nullptr;
    std::unique_ptr<zmq::socket_t> m_socket=nullptr;
    std::string m_connectionString;

    std::map<std::string, std::list<zmqCallbackFunction>> m_registeredFunctions;
    std::mutex m_registeredFunctionsMutex;

    std::list<zmq::message_t>  m_messageProcessingQueue;
    std::mutex m_messageProcessingQueueMutex;
    std::condition_variable m_notifier;
    std::mutex m_notifierMutex;
};

#endif // ZMQSUBSCRIBER_H
