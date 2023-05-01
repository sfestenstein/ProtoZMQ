#include <unistd.h>
#include <iostream>
#include <sstream>
#include <chrono>

#include <zmq.hpp>

#include <AllZmqMessages.h>
#include <MessageHeartbeat.pb.h>
#include <MessageOne.pb.h>
#include <MessageTwo.pb.h>
#include <ZmqSubscriber.h>

void processHeartbeat(void* data, size_t dataSize)
{
    MessageHeartbeat heartbeatMsg;
    heartbeatMsg.ParseFromArray(data, dataSize);
    std::cout << "Recieved Heartbeat From Callback! data size = " <<  dataSize << std::endl;
    std::cout << heartbeatMsg.DebugString() << std::endl;
}

void processMessageOne(void* data, size_t dataSize)
{
    MessageOne messageOne;
    messageOne.ParseFromArray(data, dataSize);
    std::cout << "Receive MessageOne! data size = " <<  dataSize << std::endl;
    std::cout << messageOne.DebugString() << std::endl;
}

int main (int argc, char* argv[])
{
    // Context
    zmq::context_t ctx(1);

    // Create a socket and set its identity attribute
    zmq::socket_t socket(ctx, /*ZMQ_DEALER*/ ZMQ_PUB);

    std::stringstream lcConnectionSs;
    lcConnectionSs << "tcp://localhost:5571";

    if (argc == 1)
    {
        std::cerr << "Using default options!  connection string = "
                  << lcConnectionSs.str() << std::endl;
    }
    else if (argc == 3)
    {
        lcConnectionSs.str("");
        lcConnectionSs << "tcp://" << argv[1] << ":" << argv[2];
        std::cerr << "Using User Specified options!  connection string = "
                  << lcConnectionSs.str() << std::endl;
    }
    else
    {
        std::cerr << "Invalid options!  usage:\n TestPublisher <Proxy Host> <Proxy Front End Port"
                  << std::endl;
    }

    ZmqSubscriber subscriber(lcConnectionSs.str());
    sleep(1);

    subscriber.subscribeToTopic(AllZmqMessages::MessageEnums::MESSAGE_HEARTBEAT,
                processHeartbeat);
    subscriber.subscribeToTopic(AllZmqMessages::MessageEnums::MESSAGE_ONE,
                processMessageOne);
    std::cout << "Hit any key to exit...";
    std::string anyKey;
    std::cin >> anyKey;

    return 0;
}
