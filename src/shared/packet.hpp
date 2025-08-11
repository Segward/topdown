#pragma once
#include <vector>
#include <string>

namespace packet
{
    enum class type : uint8_t
    {
        MESSAGE,
        DISCONNECT
    };

    struct header
    {
        type type;
        uint32_t size;
    };

    struct packet
    {
        header header;
        std::vector<uint8_t> data;
    };
}