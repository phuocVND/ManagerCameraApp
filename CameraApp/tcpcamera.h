#ifndef TCPCAMERA_H
#define TCPCAMERA_H
#include "icamera.h"

class TCPCamera : public ICamera {
public:
    TCPCamera(const std::string& ip, int port);
    ~TCPCamera() override;

    void open() override;
    void close() override;
    void capture() override;
    cv::Mat getLatestFrame() const override;
    void setName(const std::string name) override;
    std::string getName() const override;

private:
    std::string ip_;
    int port_;
    int sock_;
    std::string name_;
    cv::Mat latestFrame_;
};
#endif // TCPCAMERA_H
