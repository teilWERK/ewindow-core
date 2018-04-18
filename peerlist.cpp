#include "peerlist.h"
#include "qzeroconf.h"

#include <QNetworkInterface>

#include <QMetaProperty>

Peer::Peer(QZeroConfService& service)
{
	m_name = service.name();
	m_ip = service.ipv6();
	m_interface = QNetworkInterface::interfaceNameFromIndex(service.interfaceIndex());
	m_port = service.port();
	
	m_status = QString(service.txt()["status"]).toInt();
}

PeerList::PeerList(QObject* parent)
    : QAbstractListModel(parent)
{
}

QString Peer::key() const
{
    // Identify entries by name and interface
    return m_name + m_interface;
}


void PeerList::addPeer(QZeroConfService service)
{
	qDebug() << "PeerList::addPeer" << service;
    
    for (auto p : m_list) {
        if (p.key() == Peer(service).key()) {
            qWarning() << "PeerFinder::peerAdded: Ignoring double add of " << service;
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(service);
    endInsertRows();
}

void PeerList::updatePeer(QZeroConfService service)
{
    QMutableListIterator<Peer> i(m_list);
    while (i.hasNext()) {
        Peer& p = i.next();
        if (p.key() == Peer(service).key()) {
            beginResetModel();
            p.m_status = QString(service.txt()["status"]).toInt();
            endResetModel();
            return;
        }
    }
    qWarning() << "PeerList::updatePeer: Could not find entry for " << service;
}

void PeerList::removePeer(QZeroConfService service)
{
    qInfo() << "PeerList::removePeer" << service;
    QMutableListIterator<Peer> i(m_list);
    while (i.hasNext()) {
        Peer& p = i.next();
        if (p.key() == Peer(service).key()) {
            beginResetModel();
            i.remove();
            endResetModel();
            return;
        }
    }

    qWarning() << "PeerList::removePeer: Could not find entry for " << service;
}

int PeerList::rowCount(const QModelIndex& index) const
{
    return m_list.count();
}

QVariant PeerList::data(const QModelIndex& index, int role) const
{
    const Peer* peer = &m_list[index.row()];
    return Peer::staticMetaObject.property(role).readOnGadget(peer);
}

QHash<int, QByteArray> PeerList::roleNames() const
{
    QHash<int, QByteArray> roles;

    const QMetaObject metaobject = Peer::staticMetaObject;
    for (int i = 0; i < metaobject.propertyCount(); i++) {
        QMetaProperty metaproperty = metaobject.property(i);
        const char *name = metaproperty.name();
        roles[i] = name;
    }

    return roles;
}
