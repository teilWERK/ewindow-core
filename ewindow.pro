TARGET = ewindow
TEMPLATE = app

QT += qml quick
CONFIG += c++11 debug

INCLUDEPATH += re/include rem/include baresip/include
SOURCES += main.cpp \
    yuvtexturematerial.cpp \
    contactlistmodel.cpp \
    useragent.cpp \
    baresipcore.cpp \
    baresipvidisp.h baresipvidisp.cpp

HEADERS += \
    baresipvidisp.h \
    yuvtexturematerial.h \
    contactlistmodel.h \
    useragent.h \
    baresipcore.h


LIBS += -Lre -Lrem -Lbaresip
LIBS += -lre -lrem -lbaresip -lssl -lcrypto

# Installation: Build ewindow binary and libbaresip.so
INSTALLS += ewindow libbaresip

# Copy files all needed files into one directory in /opt
ewindow.path = /opt/ewindow
ewindow.files = ewindow gui
ewindow.depends = baresip/libbaresip.so

QMAKE_RPATHDIR += . # Adjust rpath so so binary will find libbaresip.so

# Build static versions of re/rem, so we don't have to deal with more .so's
re.target = re/libre.a
re.commands = make -C re libre.a
re.depends = git
rem.target = rem/librem.a
rem.commands = make -C rem librem.a

# Build libbaresip.so, STATIC=1 means all modules are compiled in the binary
libbaresip.target = baresip/libbaresip.so
libbaresip.path = /opt/ewindow
libbaresip.files = baresip/libbaresip.so
libbaresip.commands = make -C baresip STATIC=1 libbaresip.so
libbaresip.depends = re/libre.a rem/librem.a

# Get the libraries, this should be replaced with something more robust
git.target = re
git.commands = \	
	git clone https://github.com/creytiv/re && \
	git clone https://github.com/creytiv/rem && \
	git clone https://github.com/alfredh/baresip


PRE_TARGETDEPS += baresip/libbaresip.so
QMAKE_EXTRA_TARGETS += re rem libbaresip
