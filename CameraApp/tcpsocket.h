#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
class TCPSocket
{
    private:
        int sock;
        std::string ip;
        int port;
        bool initialized;
        sockaddr_in serverAddr{};

    public:
        TCPSocket(): sock(-1), port(0), ip("0.0.0.0"), initialized(false){};
        void init(const std::string &serverIp, int serverPort) {
            ip = serverIp;
            port = serverPort;
            initialized = true;
        }
        // Hàm open để tạo socket và kết nối
        bool open() {
            if (!initialized) {
                std::cerr << "Chưa init!\n";
                return false;
            }

            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("socket");
                return false;
            }

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);

            if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
                perror("inet_pton");
                return false;
            }

            if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                perror("connect");
                return false;
            }

            std::cout << "Kết nối thành công tới " << ip << ":" << port << "\n";
            return true;
        }

        // Hàm close để đóng socket
        void closeClient() {
            if (sock != -1) {
                close(sock);
                sock = -1;
                std::cout << "Đã đóng kết nối\n";
            }
        }

        // Hàm gửi dữ liệu
        bool sendData(const char* data, ssize_t length) {
            if (sock == -1) return false;
            return send(sock, data, length, 0) >= 0;
        }

        bool receiveData(char* data, size_t length) {
            if (sock == -1) return false;
            char* buffer[1024];
            ssize_t bytes = recv(sock, buffer, length, 0);
            if (bytes > 0) {
                *buffer[bytes] = '\0';
            }
            return true;
        }

        // send an integer value
        bool sendValue(const int32_t value) {
            if (sock == -1) return false;
            int32_t data = htonl(value);
            return send(sock, &data, sizeof(data), 0) >= 0;
        }

        //recv an integer value
        bool receiveValue(int32_t& value){
            if (sock == -1) return false;
            ssize_t bytes = recv(sock, &value, sizeof(int32_t), 0);
            if (bytes < 0) {
                return false;
            }
            value = ntohl(value);
            return true;
        }

        bool sendIamge(uint8_t*& data, uint32_t size) {
            if (sock == -1) return false;
            uint32_t total = 0;
            while (total < size) {
                ssize_t n = send(sock, data + total, size - total, 0);
                if (n <= 0) return false;
                total += n;
            }
            return true;
        }

        bool receiveIamge(uint8_t*& data, uint32_t size){
            if (sock == -1) return false;
            delete[] data;
            data = new uint8_t[size];
            uint32_t total = 0;
            while (total < size) {
                ssize_t n = recv(sock, data + total, size, 0);
                if (n <= 0) return false;
                total += n;
            }
            return true;
        }
};

#endif // TCPSOCKET_H
