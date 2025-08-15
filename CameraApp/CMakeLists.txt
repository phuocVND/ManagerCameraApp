cmake_minimum_required(VERSION 3.16)

project(CameraApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appCameraApp
    main.cpp
)

qt_add_qml_module(appCameraApp
    URI CameraApp
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES icamera.h icamera.cpp
        SOURCES tcpcamera.h tcpcamera.cpp
        SOURCES udpcamera.h udpcamera.cpp
        SOURCES tcpsocket.h tcpsocket.cpp
        SOURCES udpsocket.h udpsocket.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCameraApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCameraApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCameraApp
    PRIVATE Qt6::Quick ${OpenCV_LIBS}
)

include(GNUInstallDirs)
install(TARGETS appCameraApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
