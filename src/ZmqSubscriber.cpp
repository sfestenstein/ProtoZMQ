#include <ZmqSubscriber.h>

#include <MessageHeartbeat.pb.h>

ZmqSubscriber::ZmqSubscriber(std::string proxyConnectionString) :
    m_socket(m_context, ZMQ_SUB),
    m_connectionString(proxyConnectionString)
{
    m_socket.connect(m_connectionString.c_str());
    m_receiveThread = std::thread(&ZmqSubscriber::receiveThread, this);
    m_processingThread = std::thread(&ZmqSubscriber::processingThread, this);
}

ZmqSubscriber::~ZmqSubscriber()
{
    m_running = false;
    std::lock_guard<std::mutex> lockGuard(m_messageProcessingQueueMutex);
    m_context.close();
    m_receiveThread.join();
    m_messageProcessingQueue.clear();
    m_notifier.notify_all();
    m_processingThread.join();
}

void ZmqSubscriber::subscribeToTopic(const AllZmqMessages::MessageEnums messageType,
                                     std::function<void(void*, size_t)> function)
{
    std::string topic = AllZmqMessages::GetEnumString(messageType).data();

    std::lock_guard<std::mutex> lockGuard(m_registeredFunctionsMutex);
    m_registeredFunctions[topic].push_back(function);
    m_socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size()-1);
}

void ZmqSubscriber::processingThread()
{
    while (m_running)
    {
        std::unique_lock<std::mutex> lockGuard(m_notifierMutex);
        m_notifier.wait(lockGuard);
        processMessagesOffList();
    }
}

void ZmqSubscriber::processMessagesOffList()
{
    while (!m_messageProcessingQueue.empty())
    {
        zmq::message_t topicMessage;
        zmq::message_t gpbMessage;
        {
            std::lock_guard<std::mutex> lockGuard(m_messageProcessingQueueMutex);
            topicMessage = std::move(m_messageProcessingQueue.front());
            m_messageProcessingQueue.pop_front();
            gpbMessage = std::move(m_messageProcessingQueue.front());
            m_messageProcessingQueue.pop_front();
        }

        auto topicString = std::string(static_cast<char*>(topicMessage.data()), topicMessage.size());

        std::lock_guard<std::mutex> functionMapLockGuard(m_registeredFunctionsMutex);
        auto functionsToCall = m_registeredFunctions.find(topicString);
        if (functionsToCall == m_registeredFunctions.end()) continue;

        for (auto functionToCall : (*functionsToCall).second)
        {
            functionToCall(gpbMessage.data(), gpbMessage.size());
        }
    }
}


void ZmqSubscriber::receiveThread()
{
    while (m_running)
    {
        zmq::message_t topicMessage;
        zmq::message_t gpbMessage;
        if (m_socket.recv(topicMessage))
        {
            m_socket.recv(gpbMessage);

            {
                // Future Work!  Detect if list is backing up!  High Water Mark detection?
                std::lock_guard<std::mutex> lockGuard(m_messageProcessingQueueMutex);
                m_messageProcessingQueue.emplace_back(std::move(topicMessage));
                m_messageProcessingQueue.emplace_back(std::move(gpbMessage));
            }

            m_notifier.notify_all();
        }
    }

}
