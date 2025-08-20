#ifndef UDPCAMERA_H
#define UDPCAMERA_H

#include "icamera.h"
#include <QImage>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "udpsocket.h"

class UDPCamera : public ICamera {
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
public:
    UDPCamera(const std::string& ip, int port);
    ~UDPCamera() override;

    void open() override;
    void close() override;
    void capture() override;
    void show() override;
    cv::Mat getLatestFrame() const override;
    void setName(const std::string name) override;
    std::string getName() const override;
    void captureLoop();
    std::thread m_captureThread;
    std::atomic<bool> m_running{false};
    QImage matToQImage(const cv::Mat &mat) {
        switch (mat.type()) {
        case CV_8UC1: // grayscale
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
        case CV_8UC3: // BGR
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888).copy();
        case CV_8UC4: // BGRA
            return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA8888).copy();
        default:
            return QImage();
        }
    }
    QImage frame() const { return m_qimage; }
signals:
    void frameChanged();
private:
    bool m_isConnect;
    UDPSocket m_udpSocket;
    std::string m_name;
    cv::Mat m_Frame;
    cv::Mat m_latestFrame;
    QImage m_qimage;
};

#endif // UDPCAMERA_H
