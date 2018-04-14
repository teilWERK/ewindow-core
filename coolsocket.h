#pragma once

#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

class CoolSocket : public QTcpServer {
    Q_OBJECT
public:
    Q_PROPERTY(quint16 listenPort READ listenPort NOTIFY portChanged)

    CoolSocket(QObject* parent = nullptr);
    ~CoolSocket();

public Q_SLOTS:
    void connectTo(QHostAddress ip, int port);

private Q_SLOTS:
    void handleConnection();

Q_SIGNALS:
    void portChanged();

private:
    //void incomingConnection(qintptr socketDescriptor);

    QTcpSocket m_socket;
    quint16 listenPort();
    quint16 m_port;
};
