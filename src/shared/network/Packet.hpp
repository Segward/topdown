#pragma once

#include <vector>

// Enum class for packet types
enum class PacketType : uint16_t
{
    INVALID = 0,
    PLAYER_MOVE = 1,
    PLAYER_MESSAGE = 2,
    PLAYER_CONNECT = 3,
    PLAYER_DISCONNECT = 4
};

// Packet class representing a network packet
class packet
{
public:
    // Public methods
    packet(ssize_t s, PacketType t, const std::vector<uint8_t> &d);
    ~packet();
    void serialize(std::vector<uint8_t> &out) const;
    void deserialize(const std::vector<uint8_t> &in);

private:
    // Private members
    ssize_t size;
    PacketType type;
    std::vector<uint8_t> data;
};