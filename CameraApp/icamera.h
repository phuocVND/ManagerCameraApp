#ifndef ICAMERA_H
#define ICAMERA_H
#include <opencv2/opencv.hpp>

#include <string>
using namespace std;
class ICamera
{
public:
    virtual ~ICamera() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void capture() = 0;
    virtual cv::Mat getLatestFrame() const= 0;
    virtual void setName(const string name) = 0;
    virtual string getName() const = 0;
};

#endif // ICAMERA_H
