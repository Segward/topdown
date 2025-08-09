#pragma once

#include <cstdint>
#include <vector>

enum class PacketType : uint8_t
{
    MOVEMENT,
    MESSAGE
};

struct PacketHeader
{
    PacketType type;
    uint32_t size;
};

struct Packet
{
    PacketHeader header;
    std::vector<uint8_t> data;
};