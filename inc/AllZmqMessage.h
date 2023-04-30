#ifndef ALLZMQMESSAGES_H
#define ALLZMQMESSAGES_H

#include <array>
#include <string_view>

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
