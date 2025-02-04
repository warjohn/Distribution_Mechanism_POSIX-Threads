#include "pcapreader.h"
#include <pcap.h>
#include <stdexcept>
#include <iostream>
#include "../Packet/packet.h"


void pcapReader::openfile(const char* filename) {
    handle = pcap_open_offline(filename, errbuf);
    if (handle == nullptr) {
        std::cerr << "Ошибка при открытии pcap файла: " << errbuf << std::endl;
        throw std::runtime_error("Ошибка при открытии pcap файла");
    }
}

bool pcapReader::readfile(Dist& dist) {
    while ((packet_data = pcap_next(handle, &header)) != nullptr) {
        Packet packet;
        packet.header.insert(packet.header.end(), reinterpret_cast<const uint8_t*>(&header), reinterpret_cast<const uint8_t*>(&header) + sizeof(&header));
        packet.data.insert(packet.data.end(), packet_data, packet_data + header.len);
        packet.packet_number = header.len;
        dist.distPackets(packet);
    }
    pcap_close(handle);
    return true;
}

bool pcapReader::start(const char* filename, Dist& dist) {
    openfile(filename);
    return readfile(dist);
}