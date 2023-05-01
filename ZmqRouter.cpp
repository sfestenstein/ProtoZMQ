#include <iostream>
#include <zmq.hpp>


int main(int argc, char* argv[])
{
    // TODO!  Log Connections!
    // TODO!  Report Heartbeat status?
    zmq::context_t ctx(1);
    zmq::socket_t frontend(ctx, /*ZMQ_ROUTER*/ ZMQ_XSUB);
    zmq::socket_t backend(ctx, /*ZMQ_DEALER*/ ZMQ_XPUB);

    zmq::socket_ref frontend_ref = frontend;
    zmq::socket_ref backend_ref = backend;

    std::stringstream frontendPortSs;
    frontendPortSs << "tcp://*:";
    std::stringstream backendPortSs;
    backendPortSs << "tcp://*:";

    if (argc == 3)
    {
        frontendPortSs << argv[1];
        backendPortSs << argv[2];
        std::cerr << "binding with specified front-end port: " << frontendPortSs.str() << std::endl;
        std::cerr << "binding with specified back-end port:  " << backendPortSs.str() << std::endl;
    }
    else if (argc == 1)
    {
        frontendPortSs << 5570;
        backendPortSs << 5571;
        std::cerr << "binding with default front-end port: " << frontendPortSs.str() << std::endl;
        std::cerr << "binding with default back-end port:  " << backendPortSs.str() << std::endl;
    }
    else
    {
        std::cerr << "Usage: \n   ZmqRouter <front end port[optional]> <back end port[optional]>" << std::endl;
    }


    frontend.bind(frontendPortSs.str().c_str());
    backend.bind(backendPortSs.str().c_str());
    std::cout << "Starting proxy" << std::endl;
    zmq::proxy(frontend_ref, backend_ref, nullptr);
    std::cout << "Proxy done!" << std::endl;
    return 0;
}
