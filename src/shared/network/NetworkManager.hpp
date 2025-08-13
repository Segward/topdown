#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include "Packet.hpp"

// NetworkManager class for handling network communication
class NetworkManager
{
public:
    // Public methods
    NetworkManager();
    ~NetworkManager();
    void sendPacket(const packet &pkt);
    void receivePacket(const packet &pkt);

private:
    // Private methods
    void serializePacket(const packet &pkt, std::vector<uint8_t> &out);
    void deserializePacket(const std::vector<uint8_t> &in, packet &pkt);

    // Private members
    std::vector<int> clients;
    std::vector<std::pair<int, packet>> queue;
};