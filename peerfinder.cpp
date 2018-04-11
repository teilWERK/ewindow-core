#include "peerfinder.h"
#include "qzeroconf.h"

#include <QDataStream>
#include <QDebug>

#include <stdint.h>

PeerFinder::PeerFinder()
{
	m_zeroconf = new QZeroConf;

	connect(m_zeroconf, &QZeroConf::error, this, &PeerFinder::printError);
	
	connect(m_zeroconf, &QZeroConf::serviceAdded, this, &PeerFinder::peerAdded);
	connect(m_zeroconf, &QZeroConf::serviceUpdated, this, &PeerFinder::peerUpdated);
	connect(m_zeroconf, &QZeroConf::serviceRemoved, this, &PeerFinder::peerRemoved);
}

PeerFinder::~PeerFinder()
{
	delete m_zeroconf;
}

void PeerFinder::publish(QString hostname, QString service, uint16_t port)
{
	m_zeroconf->startServicePublish(hostname.toUtf8(), service.toUtf8(), "local", port);

	// Start and stop the browser with publish, because constructor is too early to not crash...
	m_zeroconf->startBrowser("_qtzeroconf_test._tcp", QAbstractSocket::IPv6Protocol);
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

void PeerFinder::printError(QZeroConf::error_t error) {
	qWarning() << "PeerFinder: Internal QZeroConf Error: " << error;
}

QString PeerFinder::make_key(QZeroConfService& service)
{
	// Do it like the QZeroConf resolveCallback...
	return service.name() + QString::number(service.interfaceIndex());
}

QString PeerFinder::make_key(Peer* peer)
{
	// Do it like the QZeroConf resolveCallback...
	return peer->name + QString::number(peer->interfaceIndex);
}

void PeerFinder::peerAdded(QZeroConfService service)
{
	qInfo() << "PeerFinder::peerAdded" << service;
	for (auto i : m_peers) {
		if (make_key(i) == make_key(service)) {
			qWarning() << "PeerFinder::peerAdded: Ignoring double add of " << service;
			return;
		}
	}
	
	Peer* peer = new Peer;
	peer->name = service.name();
	peer->ip = service.ipv6();
	peer->interfaceIndex = service.interfaceIndex();
	
	QDataStream ds(service.txt()["status"]);
	ds >> peer->status;

	m_peers.append(peer);
}

void PeerFinder::peerUpdated(QZeroConfService service)
{
	qInfo() << "PeerFinder::peerUpdated" << service;
	for (auto i : m_peers) {
		if (make_key(i) == make_key(service)) {
			QDataStream ds(service.txt()["status"]);
			ds >> i->status;
			return;
		}
	}
	
	qWarning() << "PeerFinder::peerUpdated: Could not find entry for " << service;
}

void PeerFinder::peerRemoved(QZeroConfService service)
{
	qInfo() << "PeerFinder::peerRemoved" << service;
	for (auto i : m_peers) {
		if (make_key(i) == make_key(service)) {
			qWarning() << "PeerFinder::peerRemove not implemented";
			//m_peers.remove(i);
			//delete i;
			return;
		}
	}
	
	qWarning() << "PeerFinder::peerRemove: Could not find entry for " << service;
}
