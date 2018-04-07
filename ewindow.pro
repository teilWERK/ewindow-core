TARGET = ewindow
TEMPLATE = app

QT += qml quick
CONFIG += c++17 debug

DEFINES += WEBRTC_POSIX

LIBS += libwebrtc_full.a -ldl -lX11


INCLUDEPATH += /usr/local/include/webrtc
SOURCES += main.cpp \
     baresipvidisp.cpp \
     yuvtexturematerial.cpp \
     baresipcore.cpp \
#    contactlistmodel.cpp \
#    useragent.cpp \
#    volumemanager.cpp \

HEADERS += \
    baresipvidisp.h \
    yuvtexturematerial.h \
    baresipcore.h
#    contactlistmodel.h \
#    useragent.h \
#    volumemanager.h \


#LIBS += -Lre -Lrem -Lbaresip
#LIBS += -lbaresip -lrem -lre -lssl -lcrypto -ldl -lz


