#pragma once

#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>

#include "qwebrtcsessiondescription.hpp"

//Q_DECLARE_METATYPE(QWebRTCSessionDescription)

class CoolSocket : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(quint16 listenPort READ listenPort NOTIFY portChanged)

    CoolSocket(QObject* parent = nullptr);
    ~CoolSocket();

public Q_SLOTS:
    //void connectTo(QHostAddress ip, int port, const QByteArray& sdp);
    void connectTo(QHostAddress ip, int port, QSharedPointer<QWebRTCSessionDescription> sdp);
    void sendAnswer(QSharedPointer<QWebRTCSessionDescription> sdp);

private Q_SLOTS:
    void handleConnection();
    void readyRead();

Q_SIGNALS:
    void portChanged();
    void receivedOffer(QHostAddress send, int sender_port, QSharedPointer<QWebRTCSessionDescription> sdp);
    void receivedAnswer(QHostAddress send, int sender_port, QSharedPointer<QWebRTCSessionDescription> sdp);

private:
    //void incomingConnection(qintptr socketDescriptor);

    QTcpServer m_server;
    QTcpSocket* m_socket;

    QByteArray m_recvBuffer;

    quint16 listenPort();
};
