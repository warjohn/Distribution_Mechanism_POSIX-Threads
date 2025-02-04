#include <iostream>
#include <pthread.h>
#include <queue>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <pthread.h>

#include "src/Packet/packet.h"
#include "src/pcapReader/pcapreader.h"
#include "src/Dist/dist.h"
#include "src/Handlers/handlers.h"

int main (int argc, char** argv) {
    if (argc != 2) {
        throw std::runtime_error("Error with arguments count");
    }

    std::queue<Packet> queue1, queue2, queue3;
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
    pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
    pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;

    bool Flag = false;

    Handler1 handler1(queue1, mutex1, cond1);
    Handler2 handler2(queue2, mutex2, cond2);
    Handler3 handler3(queue3, mutex3, cond3);

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, nullptr, Handler1::start_process_1, &handler1);
    pthread_create(&thread2, nullptr, Handler2::start_process_2, &handler2);
    pthread_create(&thread3, nullptr, Handler3::start_process_3, &handler3);

    std::this_thread::sleep_for(std::chrono::seconds(2)); //задержка чтобы потоки обработчиков запустились корректно 

    Dist dist(queue1, queue2, queue3, mutex1, mutex2, mutex3, cond1, cond2, cond3);
    pcapReader pcap;
    Flag = pcap.start(argv[1], dist);

    if (Flag) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); //задержка чтобы всё успело обработаться и сохраниться 
        pthread_cancel(thread1);
        pthread_cancel(thread2);
        pthread_cancel(thread3);
    }

    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    pthread_join(thread3, nullptr);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_mutex_destroy(&mutex3);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
    pthread_cond_destroy(&cond3);


    return 0;
}

