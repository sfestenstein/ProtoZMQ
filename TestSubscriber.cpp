#include <unistd.h>
#include <iostream>
#include <sstream>
#include <chrono>

#include <zmq.hpp>

#include <MessageHeartbeat.pb.h>
#include <MessageOne.pb.h>
#include <MessageTwo.pb.h>
#include <ZmqSubscriber.h>

int main (int argc, char* argv[])
{
    // Context
    zmq::context_t ctx(1);

    // Create a socket and set its identity attribute
    zmq::socket_t socket(ctx, /*ZMQ_DEALER*/ ZMQ_PUB);

    const int myPid = getpid();

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
        std::cerr << "Invalid options!  usage\n TestPublisher <Proxy Host> <Proxy Front End Port"
                  << std::endl;
    }

    ZmqSubscriber subscriber(lcConnectionSs.str());
    std::cout << "Hit any key to exit...";
    std::string anyKey;
    std::cin >> anyKey;

    return 0;
}
