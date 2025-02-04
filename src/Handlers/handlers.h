#ifndef HANDLER_H
#define HANDLER_H

#include <queue>
#include <pthread.h>
#include "../Packet/packet.h"  

class Handler1 {
public:
    Handler1(std::queue<Packet>& queue1, pthread_mutex_t& mutex1, pthread_cond_t& cond1);

    static void* start_process_1(void* arg); 
    void* process_1();  

private:
    std::queue<Packet>& queue1;
    pthread_mutex_t& mutex1;
    pthread_cond_t& cond1;
};

class Handler2 {
public:
    Handler2(std::queue<Packet>& queue2, pthread_mutex_t& mutex2, pthread_cond_t& cond2);

    static void* start_process_2(void* arg);
    void* process_2();

private:
    std::queue<Packet>& queue2;
    pthread_mutex_t& mutex2;
    pthread_cond_t& cond2;
};

class Handler3 {
public:
    Handler3(std::queue<Packet>& queue3, pthread_mutex_t& mutex3, pthread_cond_t& cond3);

    static void* start_process_3(void* arg);
    void* process_3();

private:
    std::queue<Packet>& queue3;
    pthread_mutex_t& mutex3;
    pthread_cond_t& cond3;
};

#endif // HANDLER_H
