#ifndef PCAPREADER_H
#define PCAPREADER_H

#include "pcap.h"
#include "../Dist/dist.h"

class pcapReader {
public:  

    bool start(const char* filename, Dist& dist);
    
private:
    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    const uint8_t* packet_data;

    void openfile(const char* filename);
    bool readfile(Dist& dist);

};


#endif // PCAPREADER_H