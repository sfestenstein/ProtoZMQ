#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

#include <thread>
#include <memory>

#include <zmq.hpp>

class ZmqSubscriber
{
public:
    ZmqSubscriber(std::string proxyConnectionString);
private:
    void receiveThread();

    std::thread zmqReceiveThread;
    bool m_running = true;
    std::unique_ptr<zmq::context_t> m_context=nullptr;
    std::unique_ptr<zmq::socket_t> m_socket=nullptr;
    std::string m_connectionString;
};

#endif // ZMQSUBSCRIBER_H
