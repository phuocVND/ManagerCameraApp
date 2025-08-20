import QtQuick
import QtQuick.Controls
import CameraApp

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    UDPCamera {
        id: udpCam
        Component.onCompleted: {
            udpCam.open()
            udpCam.captureLoop()
        }
    }

    Image {
        anchors.fill: parent
        source: udpCam.frame   // ⚠️ Không dùng được trực tiếp
    }
}
