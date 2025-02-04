#include "dist.h"
#include <pthread.h>
#include <queue>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <iostream>


Dist::Dist(std::queue<Packet>& queue1, std::queue<Packet>& queue2, std::queue<Packet>& queue3, 
            pthread_mutex_t& mutex1, pthread_mutex_t& mutex2, pthread_mutex_t& mutex3,
            pthread_cond_t& cond1, pthread_cond_t& cond2, pthread_cond_t& cond3) :
            queue1(queue1), queue2(queue2), queue3(queue3), mutex1(mutex1), mutex2(mutex2), mutex3(mutex3), cond1(cond1), cond2(cond2), cond3(cond3) {
            }

void Dist::distPackets(Packet packet) {
    struct ip* ip_header = (struct ip*)packet.data.data();
    struct udphdr* udp_header = (struct udphdr*)(packet.data.data() + sizeof(struct ip));

    if (ip_header->ip_dst.s_addr >= htonl(inet_addr("11.0.0.3")) && ip_header->ip_dst.s_addr <= htonl(inet_addr("11.0.0.200"))) {
        pthread_mutex_lock(&mutex1);
        queue1.push(packet);
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mutex1);
    } else if (ip_header->ip_dst.s_addr >= htonl(inet_addr("12.0.0.3")) && ip_header->ip_dst.s_addr <= htonl(inet_addr("12.0.0.200")) &&
               ntohs(udp_header->dest) == 8080) {
        pthread_mutex_lock(&mutex2);
        queue2.push(packet);
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mutex2);
    } else {
        pthread_mutex_lock(&mutex3);
        queue3.push(packet);
        pthread_cond_signal(&cond3);    
        pthread_mutex_unlock(&mutex3);
    }
}