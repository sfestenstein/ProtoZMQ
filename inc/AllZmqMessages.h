#ifndef ALLZMQMESSAGES_H
#define ALLZMQMESSAGES_H

#include <array>
#include <string_view>

#include <zmq.hpp>

class AllZmqMessages
{
public:
    enum class MessageEnums
    {
        MESSAGE_ONE,
        MESSAGE_TWO,
        MESSAGE_HEARTBEAT,
        NUM_MESSAGES
    };
    static constexpr size_t NumberOfEnums = 3;

    // Strings to be used as topics
    static constexpr std::array<std::string_view, NumberOfEnums> EnumStrings =
    {{"One", "Two", "Heartbeat"}};

    // Strings to be used as topics
    // Future Work, can we use a constexpr function to do this for us?
    static constexpr std::array<zmq::const_buffer, NumberOfEnums> TopicMessages =
     {{zmq::const_buffer(EnumStrings[0].data(), EnumStrings[0].size()),
       zmq::const_buffer(EnumStrings[1].data(), EnumStrings[1].size()),
       zmq::const_buffer(EnumStrings[2].data(), EnumStrings[2].size())}};
    
    static constexpr std::string_view GetEnumString(MessageEnums messageType)
    {
        return EnumStrings.at(static_cast<size_t>(messageType));
    }

    AllZmqMessages()
    {
        static_assert((NumberOfEnums == static_cast<size_t>(MessageEnums::NUM_MESSAGES)),
                       "Problem with MessageEnums!");
    }

};

#endif // ALLZMQMESSAGES_H
