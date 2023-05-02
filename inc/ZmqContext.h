#ifndef ZMQCONTEXT_H
#define ZMQCONTEXT_H

#include <zmq.hpp>

class ZmqContext
{
protected:
    static zmq::context_t m_context;
};

#endif // ZMQCONTEXT_H