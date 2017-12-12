TARGET = ewindow
TEMPLATE = app

QT += qml quick
CONFIG += c++11 debug
QMAKE_RPATHDIR += ../lib baresip

INCLUDEPATH += /usr/local/include/re /usr/local/include/rem
SOURCES += main.cpp \
    yuvtexturematerial.cpp \
    contactlistmodel.cpp \
    useragent.cpp \
    baresipcore.cpp \
    baresipvidisp.h baresipvidisp.cpp

LIBS += -Lre -Lrem -Lbaresip
LIBS += -lre -lrem -lbaresip

re.target = re/libre.a
re.commands = make -C re
rem.target = rem/librem.a
rem.commands = make -C rem

baresip.target = baresip/libbaresip.so
baresip.commands = make -C baresip STATIC=1 libbaresip.so
baresip.depends = re/libre.a rem/librem.a
baresip.path = ${QT_INSTALL_LIBS}
baresip.files = $$baresip.target

ewindow.depends = baresip/baresip.so

PRE_TARGETDEPS += baresip/libbaresip.so
QMAKE_EXTRA_TARGETS += re rem baresip

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    baresipvidisp.h \
    yuvtexturematerial.h \
    contactlistmodel.h \
    useragent.h \
    baresipcore.h

DISTFILES += \
    main.qml \
    ContactList.qml \
    TimeoutDialog.qml
