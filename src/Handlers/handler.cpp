#include "handlers.h"
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <queue>
#include <pthread.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

#include "../Packet/packet.h"


/*Функции корректной записи pcap файлов*/
void writePcapHeader(std::ofstream& outfile) {
    uint32_t magic_number = 0xA1B2C3D4;  // Magic number для pcap файла
    uint16_t version_major = 2;          // Версия
    uint16_t version_minor = 4;          // Версия
    uint32_t thiszone = 0;               // Временная зона
    uint32_t sigfigs = 0;                // Значение времени для точности
    uint32_t snaplen = 65535;            // Максимальный размер пакета
    uint32_t network = 1;                // Тип сети (например, Ethernet)

    outfile.write(reinterpret_cast<const char*>(&magic_number), sizeof(magic_number));
    outfile.write(reinterpret_cast<const char*>(&version_major), sizeof(version_major));
    outfile.write(reinterpret_cast<const char*>(&version_minor), sizeof(version_minor));
    outfile.write(reinterpret_cast<const char*>(&thiszone), sizeof(thiszone));
    outfile.write(reinterpret_cast<const char*>(&sigfigs), sizeof(sigfigs));
    outfile.write(reinterpret_cast<const char*>(&snaplen), sizeof(snaplen));
    outfile.write(reinterpret_cast<const char*>(&network), sizeof(network));
}

void writePacket(std::ofstream& outfile, const Packet& packet) {
    // Пример заголовка пакета
    uint32_t ts_sec = 0;        // Время захвата в секундах
    uint32_t ts_usec = 0;       // Время захвата в микросекундах
    uint32_t incl_len = packet.header.size() + packet.data.size(); // Длина пакета с данными
    uint32_t orig_len = incl_len; // Оригинальная длина пакета

    outfile.write(reinterpret_cast<const char*>(&ts_sec), sizeof(ts_sec));
    outfile.write(reinterpret_cast<const char*>(&ts_usec), sizeof(ts_usec));
    outfile.write(reinterpret_cast<const char*>(&incl_len), sizeof(incl_len));
    outfile.write(reinterpret_cast<const char*>(&orig_len), sizeof(orig_len));

    outfile.write(reinterpret_cast<const char*>(packet.header.data()), packet.header.size());
    outfile.write(reinterpret_cast<const char*>(packet.data.data()), packet.data.size());
}

void writeToFile(const std::string& filename, const Packet& packet) {
    std::ofstream outfile(filename, std::ios::binary | std::ios::app);
    if (outfile.tellp() == 0) {
        writePcapHeader(outfile);
    }
    writePacket(outfile, packet);
}


/*конструкторы обработчиков*/
Handler1::Handler1(std::queue<Packet>& queue1, pthread_mutex_t& mutex1, pthread_cond_t& cond1) : queue1(queue1), mutex1(mutex1), cond1(cond1) {};

void* Handler1::start_process_1(void* arg) {
    return static_cast<Handler1*>(arg)->process_1();
}

Handler2::Handler2(std::queue<Packet>& queue2, pthread_mutex_t& mutex2, pthread_cond_t& cond2) : queue2(queue2), mutex2(mutex2), cond2(cond2) {};

void* Handler2::start_process_2(void* arg) {
    return static_cast<Handler2*>(arg)->process_2();
}

Handler3::Handler3(std::queue<Packet>& queue3, pthread_mutex_t& mutex3, pthread_cond_t& cond3) : queue3(queue3), mutex3(mutex3), cond3(cond3) {};

void* Handler3::start_process_3(void* arg) {
    return static_cast<Handler3*>(arg)->process_3();
}


/*потоковые*/

void* Handler1::process_1() {
    while (true) {
        pthread_mutex_lock(&mutex1);
        while (queue1.empty()) {
            pthread_cond_wait(&cond1, &mutex1);
        }
        Packet packet = queue1.front();
        queue1.pop();
        pthread_mutex_unlock(&mutex1);

        struct ip* ip_header = (struct ip*)packet.data.data();
        if (ip_header->ip_dst.s_addr >= htonl(inet_addr("11.0.0.3")) && ip_header->ip_dst.s_addr <= htonl(inet_addr("11.0.0.200"))) {
            struct udphdr* udp_header = (struct udphdr*)(packet.data.data() + sizeof(struct ip));
            if (ntohs(udp_header->dest) != 7070) {
                writeToFile("result_1.pcap", packet);
            } else {
                std::cout << "Обработчик 1: пакет под номером "<< packet.packet_number <<" игнорируется" <<std::endl;
            }
        }
    }
    return nullptr;
}

void* Handler2::process_2() {
    while (true) {
        pthread_mutex_lock(&mutex2);
        while (queue2.empty()) {
            pthread_cond_wait(&cond2, &mutex2);
        }
        Packet packet = queue2.front();
        queue2.pop();
        pthread_mutex_unlock(&mutex2);

        struct ip* ip_header = (struct ip*)packet.data.data();
        struct udphdr* udp_header = (struct udphdr*)(packet.data.data() + sizeof(struct ip));
        if (ip_header->ip_dst.s_addr >= htonl(inet_addr("12.0.0.3")) && ip_header->ip_dst.s_addr <= htonl(inet_addr("12.0.0.200")) &&
            ntohs(udp_header->dest) == 8080) {
            
            std::vector<uint8_t>::iterator pos = std::find(packet.data.begin(), packet.data.end(), 'x');
            if (pos != packet.data.end()) {
                packet.data.erase(pos + 1, packet.data.end());
            }
            writeToFile("result_2.pcap", packet);
        }
    }
    return nullptr;
}

void* Handler3::process_3() {
    while (true) {
        pthread_mutex_lock(&mutex3);
        while (queue3.empty()) {
            pthread_cond_wait(&cond3, &mutex3);
        }
        Packet packet = queue3.front();
        queue3.pop();
        pthread_mutex_unlock(&mutex3);

        struct ip* ip_header = (struct ip*)packet.data.data();
        if (packet.data.size() > sizeof(struct ip)) {
            if (ip_header->ip_p == IPPROTO_TCP) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                time_t current_time = time(nullptr);
                if (current_time % 2 == 0) {
                    writeToFile("result_3.pcap", packet);
                }
            } else if (ip_header->ip_p == IPPROTO_UDP) {
                struct udphdr* udp_header = (struct udphdr*)(packet.data.data() + sizeof(struct ip));
                if (ntohs(udp_header->source) == ntohs(udp_header->dest)) {
                    writeToFile("result_3.pcap", packet);
                    std::cout << "Обработчик 3: Найдено совпадение port = " << ntohs(udp_header->source) << std::endl;
                }
            }
        }
    }
    return nullptr;
}