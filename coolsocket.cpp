#include "coolsocket.h"

#include <QNetworkConfigurationManager>

CoolSocket::CoolSocket(QObject* parent)
    : QTcpServer(parent)
{
    QNetworkConfigurationManager netman;
    qInfo() << "ONLINE: " << netman.isOnline();
    QNetworkConfiguration netconf = netman.defaultConfiguration();
    qInfo() << netconf.bearerTypeName();

    connect(this, &CoolSocket::newConnection, this, &CoolSocket::handleConnection);

    listen();
}

CoolSocket::~CoolSocket()
{
}


quint16 CoolSocket::listenPort()
{
    return serverPort();
}

void CoolSocket::handleConnection()
{
    qDebug() << "handleConnection";

    QTcpSocket* socket = nextPendingConnection();

    qDebug() << socket;
}

void CoolSocket::connectTo(QHostAddress ip, int port)
{
    qDebug() << "CoolSocket connecting to " << ip << ":" << port;
    m_socket.connectToHost(ip, port);
}
