#ifndef BARESIPCORE_H
#define BARESIPCORE_H

#include <QThread>

class BaresipVidisp;

class BaresipCore : public QThread {
    Q_OBJECT

signals:
    void newVideo(BaresipVidisp* video);

public:
    static BaresipCore& instance() { static BaresipCore instance; return instance; }
    void run();

private:
    BaresipCore() = default;
};


#endif // BARESIPCORE_H
