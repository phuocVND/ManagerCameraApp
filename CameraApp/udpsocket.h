#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#define MAX_UDP_SIZE 1400
using namespace std;

class UDPSocket {
private:
    int sock;
    std::string ip;
    int port;
    bool initialized;
    sockaddr_in serverAddr;
public:
    UDPSocket() : sock(-1), port(0), ip("0.0.0.0"), initialized(false) {}
    UDPSocket(const std::string &serverIp, int serverPort)
        : sock(-1), ip(serverIp), port(serverPort), initialized(false){init();}
    void init() {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            perror("inet_pton");
        }
        initialized = true;
    }
    bool connect() {
        if (!initialized) {
            cerr << "Chưa init!\n";
            return false;
        }

        sock = socket(AF_INET, SOCK_DGRAM, 0); // UDP
        if (sock < 0) {
            perror("socket");
            return false;
        }

        cout << "UDP socket đã mở tới " << ip << ":" << port << "\n";
        return true;
    }

    void closeSocket() {
        if (sock != -1) {
            close(sock);
            sock = -1;
            cout << "Đã đóng socket\n";
        }
    }

    bool sendData(const void* data, ssize_t length) {
        if (sock == -1) return false;
        const char* ptr = (const char*)data;
        size_t sent = 0;

        while (sent < length) {
            size_t chunk = std::min((size_t)MAX_UDP_SIZE, (size_t)length - sent);
            ssize_t n = sendto(sock, ptr + sent, chunk, 0,
                               (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            if (n <= 0) return false; // lỗi khi gửi
            sent += n;
        }

        return true;
    }

    bool receiveData(void* buffer, size_t length) {
        if (sock == -1) return false;
        socklen_t addrLen = sizeof(serverAddr);
        size_t received = 0;

        while (received < length) {
            size_t chunk = std::min((size_t)MAX_UDP_SIZE, length - received);
            ssize_t n = recvfrom(sock, (char*)buffer + received, chunk, 0,
                                 (struct sockaddr*)&serverAddr, &addrLen);
            if (n <= 0) return false;
            received += n;
        }

        return true;
    }


    bool sendValue(int32_t &value) {
        int32_t data = htonl(value);
        return sendData(&data, sizeof(int32_t));
    }

    bool receiveValue(int32_t &value) {
        int32_t data;
        if (!receiveData(&data, sizeof(int32_t))) return false;
        value = ntohl(data);
        return true;
    }
};

#endif // UDPSOCKET_H
