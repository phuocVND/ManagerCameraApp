#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
using namespace std;
class UDPSocket
{
private:
    int sock;
    std::string ip;
    int port;
    bool initialized;
    sockaddr_in serverAddr;
public:
    UDPSocket() : sock(-1), port(0), ip("0.0.0.0"), initialized(false) {}
    void init(const std::string &serverIp, int serverPort) {
        ip = serverIp;
        port = serverPort;
        initialized = true;

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            perror("inet_pton");
        }
    }
    bool open() {
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

    bool sendData(const char* data, ssize_t length) {
        if (sock == -1) return false;
        ssize_t n = sendto(sock, data, length, 0,
                           (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        return n >= 0;
    }

    bool receiveData(char* buffer, size_t length) {
        if (sock == -1) return false;
        socklen_t addrLen = sizeof(serverAddr);
        ssize_t n = recvfrom(sock, buffer, length, 0,
                             (struct sockaddr*)&serverAddr, &addrLen);
        if (n >= 0) {
            buffer[n] = '\0';
            return true;
        }
        return false;
    }

    bool sendValue(int32_t value) {
        int32_t data = htonl(value);
        return sendData((char*)&data, sizeof(data));
    }

    bool receiveValue(int32_t &value) {
        int32_t data;
        if (!receiveData((char*)&data, sizeof(data))) return false;
        value = ntohl(data);
        return true;
    }
};

#endif // UDPSOCKET_H
