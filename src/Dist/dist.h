#ifndef DIST_H
#define DIST_H

#include "../Packet/packet.h"
#include <queue>
#include <pthread.h>

class Dist {

public:

    Dist(std::queue<Packet>& queue1, std::queue<Packet>& queue2, std::queue<Packet>& queue3, 
            pthread_mutex_t& mutex1, pthread_mutex_t& mutex2, pthread_mutex_t& mutex3,
            pthread_cond_t& cond1, pthread_cond_t& cond2, pthread_cond_t& cond3);

    void distPackets(Packet paclet);
    
    std::queue<Packet>& queue1;
    std::queue<Packet>& queue2;
    std::queue<Packet>& queue3;
    pthread_mutex_t& mutex1;
    pthread_mutex_t& mutex2;
    pthread_mutex_t& mutex3;
    pthread_cond_t& cond1;
    pthread_cond_t& cond2;
    pthread_cond_t& cond3;

};

#endif //DIST_H