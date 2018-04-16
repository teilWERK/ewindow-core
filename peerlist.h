#pragma once

#include "qzeroconf.h"

#include <QAbstractListModel>

// We need to tell Qt somewhere these exist to use them in QML
Q_DECLARE_METATYPE(QHostAddress)

class QZeroConfService;

class Peer {
    Q_GADGET
    /** Think of useful presence status, besides Available and Busy/InConnection
    enum class PresenceStatus {
	AVAILABLE = 0,
	BUSY,
	INCLOSEDCONNECTION,
    };
    */
public:
    Peer() = default;
    Peer(QZeroConfService& service);
    QString key() const; // For comparing

    Q_PROPERTY(QString name MEMBER m_name);
    Q_PROPERTY(int status MEMBER m_status);
    Q_PROPERTY(QHostAddress ip MEMBER m_ip);
    Q_PROPERTY(quint16 port MEMBER m_port);
    Q_PROPERTY(QString interface MEMBER m_interface);

private:
    QString m_name;
    int m_status;
    QHostAddress m_ip;
    quint16 m_port;
    QString m_interface;

    friend class PeerList;
};

Q_DECLARE_METATYPE(Peer)

class PeerList : public QAbstractListModel
{
    Q_OBJECT
    
public:
    PeerList(QObject* parent = nullptr);
    // Qt model interface
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public Q_SLOTS:
    // convenience slots to connect to QZeroConf updates
	void addPeer(QZeroConfService service);
	void updatePeer(QZeroConfService service);
	void removePeer(QZeroConfService service);

    // TODO: add normal data manipulation methods
    
private:
    QList<Peer> m_list;
};

Q_DECLARE_METATYPE(PeerList*)
