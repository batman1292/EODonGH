# This is a qmake project file, provided as an example on how to use qmake with QtGStreamer.

TEMPLATE = app
TARGET = player

# produce nice compilation output
CONFIG += silent

# Tell qmake to use pkg-config to find QtGStreamer.
CONFIG += link_pkgconfig

# Now tell qmake to link to QtGStreamer and also use its include path and Cflags.
#contains(QT_VERSION, ^4\\..*) {
#  PKGCONFIG += QtGStreamer-1.0 QtGStreamerUi-1.0
#}
#contains(QT_VERSION, ^5\\..*) {
#  PKGCONFIG += Qt5GStreamer-1.0 Qt5GStreamerUi-1.0
#  QT += widgets
#}

# Recommended if you are using g++ 4.5 or later. Must be removed for other compilers.
QMAKE_CXXFLAGS += -std=c++0x

# Recommended, to avoid possible issues with the "emit" keyword
# You can otherwise also define QT_NO_EMIT, but notice that this is not a documented Qt macro.
DEFINES += QT_NO_KEYWORDS

# Input
HEADERS += mediaapp.h player.h
SOURCES += main.cpp mediaapp.cpp player.cpp

INCLUDEPATH += "D:\gstreamer\1.0\x86\include" \
"D:\gstreamer\1.0\x86\include\gstreamer-1.0" \
"D:\gstreamer\1.0\x86\include\glib-2.0" \
"D:\gstreamer\1.0\x86\lib\glib-2.0\include"

LIBS += -L"D:\gstreamer\1.0\x86\lib" -lglib-2.0 -lgobject-2.0 \
-lgstreamer-1.0 -lgstapp-1.0 -lgstvideo-1.0

INCLUDEPATH += "D:\EODonGH\lib\qt-gstreamer-1.2.0\src"

CONFIG += "D:\gstreamer\1.0\x86\lib\pkgconfig"

QT += widgets
