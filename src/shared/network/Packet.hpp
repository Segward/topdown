#pragma once

#include <cstdint>
#include <vector>

class packet
{
public:
    // Fields
    uint32_t size;
    uint16_t type;
    std::vector<uint8_t> data;

    // Methods
    packet();
    ~packet();
    void serialize(std::vector<uint8_t> &out) const;
    void deserialize(const std::vector<uint8_t> &in);
};