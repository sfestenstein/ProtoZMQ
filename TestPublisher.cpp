#include <unistd.h>
#include <iostream>
#include <sstream>
#include <chrono>

#include <zmq.hpp>

#include <MessageHeartbeat.pb.h>
#include <MessageOne.pb.h>
#include <MessageTwo.pb.h>
#include <ZmqPublisher.h>

int main (int argc, char* argv[])
{
    // Context
    zmq::context_t ctx(1);

    // Create a socket and set its identity attribute
    zmq::socket_t socket(ctx, /*ZMQ_DEALER*/ ZMQ_PUB);

    const int myPid = getpid();

    std::stringstream lcConnectionSs;
    lcConnectionSs << "tcp://localhost:5570";



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
        std::cerr << "Invalid options!  usage\n TestPublisher <Proxy Host> <Proxy Front End Port"
                  << std::endl;
    }

    ZmqPublisher publisher(lcConnectionSs.str());
    // Send some messages
    unsigned int counter = 0;
    while (true)
    {
        auto timeNowNs = std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
        MessageHeartbeat heartbeatMsg;
        heartbeatMsg.set_heartbeat_id(myPid);
        heartbeatMsg.set_message_time(timeNowNs);

        std::cout << "Sending Heartbeat! " << heartbeatMsg.IsInitialized() << std::endl;

        publisher.send(heartbeatMsg, AllZmqMessages::MessageEnums::MESSAGE_HEARTBEAT);
        std::cout << heartbeatMsg.DebugString() << std::endl;

        sleep(1);
    }
    return 0;
}
