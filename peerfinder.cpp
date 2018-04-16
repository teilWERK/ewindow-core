#include "peerfinder.h"
#include "peerlist.h"

#include <QDebug>

PeerFinder::PeerFinder()
{
	m_zeroconf = new QZeroConf;
	m_peerlist = new PeerList(this);

	connect(m_zeroconf, &QZeroConf::error, this, &PeerFinder::printError);
	connect(m_zeroconf, &QZeroConf::serviceAdded, m_peerlist, &PeerList::addPeer);
	connect(m_zeroconf, &QZeroConf::serviceUpdated, m_peerlist, &PeerList::updatePeer);
	connect(m_zeroconf, &QZeroConf::serviceRemoved, m_peerlist, &PeerList::removePeer);
}

PeerFinder::~PeerFinder()
{
	delete m_peerlist;
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

PeerList* PeerFinder::model()
{
	return m_peerlist;
}

void PeerFinder::printError(QZeroConf::error_t error) {
	qWarning() << "PeerFinder: Internal QZeroConf Error: " << error;
}
