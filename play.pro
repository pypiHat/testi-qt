TEMPLATE = app
QT += qml quick widgets

CONFIG += link_pkgconfig debug
PKGCONFIG = gstreamer-1.0 gstreamer-video-1.0

DEFINES += GST_USE_UNSTABLE_API

INCLUDEPATH += ../lib
SOURCES += main.cpp
RESOURCES += qmlsink-dyn-added.qrc

# Additional GStreamer paths for Windows if pkg-config fails
INCLUDEPATH += C:/gstreamer/1.0/msvc_x86_64/include
LIBS += -LC:/gstreamer/1.0/msvc_x86_64/lib -lgstreamer-1.0 -lgstvideo-1.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
