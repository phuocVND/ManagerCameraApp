#include "udpcamera.h"
#include <iostream>
#include <unistd.h>



UDPCamera::UDPCamera(const std::string& ip, int port) : m_udpSocket(ip, port)
{

}

UDPCamera::~UDPCamera() {
    close();
}

void UDPCamera::open() {
    m_isConnect = m_udpSocket.connect();
    char a[] = "Hello";
    if(m_isConnect) m_udpSocket.sendData(a, 6);
    m_running = true;
    m_captureThread = std::thread(&UDPCamera::captureLoop, this);
}

void UDPCamera::close() {
    m_running = false;
    if(m_captureThread.joinable())
        m_captureThread.join();
    m_isConnect = false;
    m_udpSocket.closeSocket();
}

void UDPCamera::capture() {
    if(!m_isConnect) return;
    int32_t size;
    m_udpSocket.receiveValue(size);
    // std::cout << (int)size << std::endl;
    std::vector<uchar> buf(size);

    if (!m_udpSocket.receiveData(buf.data(), buf.size())) {
        std::cerr << "Failed to receive frame data\n";
        return;
    }

    m_Frame = cv::imdecode(buf, cv::IMREAD_COLOR);

    if(m_Frame.empty()) {
        std::cerr << "Decode failed!" << std::endl;
        return;
    }
    m_latestFrame = m_Frame;
    m_qimage = matToQImage(m_latestFrame);
    emit frameChanged();
    // cv::imshow("Server - Sending...", m_latestFrame);
    // std::cout << (int)buf.size() << std::endl;
}
void UDPCamera::captureLoop() {
    while(m_running) {
        capture();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void UDPCamera::show(){
    while(1) capture();
}

cv::Mat UDPCamera::getLatestFrame() const {
    return m_latestFrame;
}

void UDPCamera::setName(const std::string name) {
    m_name = name;
}

std::string UDPCamera::getName() const {
    return m_name;
}
