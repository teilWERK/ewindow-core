#include "peerfinder.h"
#include "qzeroconf.h"

#include <QDataStream>
#include <QDebug>
#include <QNetworkInterface>

PeerFinder::PeerFinder()
{
	m_zeroconf = new QZeroConf;

	connect(m_zeroconf, &QZeroConf::error, this, &PeerFinder::printError);
	connect(m_zeroconf, &QZeroConf::serviceAdded, this, &PeerFinder::peerAdded);
	connect(m_zeroconf, &QZeroConf::serviceUpdated, this, &PeerFinder::peerUpdated);
	connect(m_zeroconf, &QZeroConf::serviceRemoved, this, &PeerFinder::peerRemoved);

	/* Fake entries for testing property list binding...
	Peer* fakepeer = new Peer;
	fakepeer->m_name = "foo";
	fakepeer->m_status = 1;
	m_peerlist.append(fakepeer);

	fakepeer = new Peer;
	fakepeer->m_name = "bar";
	fakepeer->m_status = 0;
	m_peerlist.append(fakepeer);
	* */
}

PeerFinder::~PeerFinder()
{
	delete m_zeroconf;
}

//void PeerFinder::publish(QString hostname, uint16_t port, QString service)
void PeerFinder::publish(QString hostname, int port, QString service)
{
	m_zeroconf->startServicePublish(hostname.toUtf8(), service.toUtf8(), "local", port);

	// Start and stop the browser with publish, because constructor is too early to not crash...
	m_zeroconf->startBrowser(service, QAbstractSocket::IPv6Protocol);
}

void PeerFinder::unpublish() {
	m_zeroconf->stopServicePublish();
	m_zeroconf->stopBrowser();
}

void PeerFinder::setStatus(int presence) {
	m_zeroconf->clearServiceTxtRecords();
	m_zeroconf->addServiceTxtRecord("status", QString::number(presence));
	m_zeroconf->updateServiceTxtRecords();
}

QQmlListProperty<Peer> PeerFinder::model()
{
	// TODO: Qt Doc says this constructor should not be used in production code
	return QQmlListProperty<Peer>(this, m_peerlist);
}

void PeerFinder::printError(QZeroConf::error_t error) {
	qWarning() << "PeerFinder: Internal QZeroConf Error: " << error;
}

QString PeerFinder::make_key(const QZeroConfService& service)
{
	// Do it like the QZeroConf resolveCallback...
	return service.name() +
		QNetworkInterface::interfaceFromIndex(service.interfaceIndex()).name();
}

QString PeerFinder::make_key(const Peer* peer)
{
	// Identify entries by name and interface
	return peer->m_name + peer->m_interface;
}

void PeerFinder::peerAdded(QZeroConfService service)
{
	qDebug() << "PeerFinder::peerAdded" << service;
	//for (auto i : m_peerlist) {
	for (QList<Peer*>::const_iterator i = m_peerlist.begin(); i != m_peerlist.end(); i++) {
		const Peer* p = static_cast<Peer*>(*i);
		if (make_key(p) == make_key(service)) {
			qWarning() << "PeerFinder::peerAdded: Ignoring double add of " << service;
			return;
		}
	}
	
	Peer* peer = new Peer;
	peer->m_name = service.name();
	peer->m_ip = service.ipv6();
	peer->m_interface = QNetworkInterface::interfaceNameFromIndex(service.interfaceIndex());
	peer->m_port = service.port();
	qDebug() << "Found interface: " << peer->m_interface;
	
	qDebug() << "Found IP: " << peer->m_ip;
	
	QDataStream ds(service.txt()["status"]);
	ds >> peer->m_status;

	m_peerlist.append(peer);
	Q_EMIT peersChanged();
}

void PeerFinder::peerUpdated(QZeroConfService service)
{
	qDebug() << "PeerFinder::peerUpdated" << service << service.txt()["status"];
	for (auto i: m_peerlist) {
		Peer* p = static_cast<Peer*>(i);
		if (make_key(p) == make_key(service)) {
			QDataStream ds(service.txt()["status"]);
			ds >> p->m_status;
			Q_EMIT p->statusChanged();
			Q_EMIT peersChanged();
			return;
		}
	}
	
	qWarning() << "PeerFinder::peerUpdated: Could not find entry for " << service;
}

void PeerFinder::peerRemoved(QZeroConfService service)
{
	qInfo() << "PeerFinder::peerRemoved" << service;
	QMutableListIterator<Peer*> i(m_peerlist);
	while (i.hasNext()) {
		Peer* p = i.next();
		if (make_key(p) == make_key(service)) {
			i.remove();
			Q_EMIT peersChanged();
			return;
		}
	}
	
	qWarning() << "PeerFinder::peerRemove: Could not find entry for " << service;
}
