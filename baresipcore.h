#ifndef BARESIPCORE_H
#define BARESIPCORE_H

#include <QThread>

class BaresipVidisp;

class BaresipCore : public QThread {
    Q_OBJECT

Q_SIGNALS:
    void newVideo(BaresipVidisp* video);

public:
    static BaresipCore& instance() { static BaresipCore instance; return instance; }
    void run() {}

public Q_SLOTS:
    void initWebRTC();

private:
    BaresipCore() = default;
};


#endif // BARESIPCORE_H
