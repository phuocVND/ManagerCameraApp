#ifndef ICAMERA_H
#define ICAMERA_H


#include <string>
using namespace std;
class ICamera
{
    public:
        ICamera();
        virtual ~ICamera() = default;
        virtual void open() = 0;
        virtual void close() = 0;
        virtual void start() = 0;
        virtual cv::Mat getLatestFrame() = 0;
        virtual void setName(string name) = 0;
        virtual string getName() = 0;
    private:
        bool isCamEnable;
        string nameCam;
};

#endif // ICAMERA_H
