#include "coolsocket.h"

#include <QNetworkConfigurationManager>

#include "qwebrtcpeerconnection.hpp"

CoolSocket::CoolSocket(QObject* parent)
    : QObject(parent)
{
    QNetworkConfigurationManager netman;
    qInfo() << "ONLINE: " << netman.isOnline();
    QNetworkConfiguration netconf = netman.defaultConfiguration();
    qInfo() << netconf.bearerTypeName();

    m_server.listen();
    connect(&m_server, &QTcpServer::newConnection, this, &CoolSocket::handleConnection);

    m_socket = nullptr;
}

CoolSocket::~CoolSocket()
{
    if (m_socket) {
        delete m_socket;
    }
}


quint16 CoolSocket::listenPort()
{
    return m_server.serverPort();
}

void CoolSocket::handleConnection()
{
    QTcpSocket* socket = m_server.nextPendingConnection();
    
    qDebug() << "handleConnection" << socket;

    // If there is an open socket, we can't handle another connection
    // TODO: Handle the case of mutual auto-connects
    if (m_socket) {
        delete socket; // Delete the new connection
        return;
    } else {
        m_socket = socket;
    }
    
    connect(m_socket, &QTcpSocket::readyRead, this, &CoolSocket::readyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &CoolSocket::peerDisconnected);

    // Call readyRead to close the connect gap
    readyRead();
}

//void CoolSocket::connectTo(QHostAddress ip, int port, const QByteArray& sdp)
void CoolSocket::connectTo(QHostAddress ip, int port, QSharedPointer<QWebRTCSessionDescription> sdp)
{
    if (m_socket) {
        delete m_socket;
    }
    m_socket = new QTcpSocket;
    connect(m_socket, &QTcpSocket::readyRead, this, &CoolSocket::readyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &CoolSocket::peerDisconnected);
    qDebug() << "CoolSocket connecting to " << ip << ":" << port;
    m_socket->connectToHost(ip, port);
    m_socket->write(sdp->sdp());
    m_socket->write("\r\n");
}

void CoolSocket::sendAnswer(QSharedPointer<QWebRTCSessionDescription> sdp)
{
    m_socket->write(sdp->sdp());
    m_socket->write("\r\n");
    qInfo() << "CoolSocket::sendAnswer";
}

void CoolSocket::readyRead()
{
    qInfo() << "Received data on socket!!!";
    m_recvBuffer += m_socket->readAll();
    if (m_recvBuffer.contains("\r\n\r\n")) {
        // Double line terminator found, signal the SDP
        qInfo() << "Received SDP!!!";
        m_recvBuffer.replace("\r\n\r\n", "\r\n"); // Trim the extra newline
        // Workaround to determine if this is an outgoing socket
        bool is_incoming_connection = m_socket->peerName() == "";
        auto type = is_incoming_connection ? QWebRTCSessionDescription::SDPType::Offer :
                QWebRTCSessionDescription::SDPType::Answer;
        auto sdp = QWebRTCPeerConnection::createSessionDescription(type, m_recvBuffer);
        m_recvBuffer.clear();
        qInfo() << "Received SDP: " << sdp;

        if (is_incoming_connection) {
            Q_EMIT receivedOffer(m_socket->peerAddress(), m_socket->peerPort(), sdp);
        } else {
            Q_EMIT receivedAnswer(m_socket->peerAddress(), m_socket->peerPort(), sdp);
        }
    }
}

void CoolSocket::peerDisconnected() {
    qInfo() << "CoolSocket::peerDisconnected";
    sender()->deleteLater();
    m_socket = nullptr;

     Q_EMIT(disconnected());
}
