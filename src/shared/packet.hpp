#pragma once
#include <vector>

namespace packet
{
    enum class type
    {
        MESSAGE
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