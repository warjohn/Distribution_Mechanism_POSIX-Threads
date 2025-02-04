#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <inttypes.h>

struct Packet {
    std::vector<uint8_t> header;
    std::vector<uint8_t> data;
    size_t packet_number;
};

#endif //PACKET_H