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
#include <ZmqContext.h>


class ZmqSubscriber : public ZmqContext
{
public:
    using zmqCallbackFunction = std::function<void(void*, size_t)>;
    ZmqSubscriber(std::string proxyConnectionString);

    virtual ~ZmqSubscriber();
    ZmqSubscriber(const ZmqSubscriber&) = delete;
    ZmqSubscriber& operator=(ZmqSubscriber&) = delete;
    ZmqSubscriber(const ZmqSubscriber&&) = delete;
    ZmqSubscriber& operator=(ZmqSubscriber&&) = delete;

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

    zmq::socket_t m_socket;
    std::string m_connectionString;

    std::map<std::string, std::list<zmqCallbackFunction>> m_registeredFunctions;
    std::mutex m_registeredFunctionsMutex;

    std::list<zmq::message_t>  m_messageProcessingQueue;
    std::mutex m_messageProcessingQueueMutex;
    std::condition_variable m_notifier;
    std::mutex m_notifierMutex;
};

#endif // ZMQSUBSCRIBER_H
