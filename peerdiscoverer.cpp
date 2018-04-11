#include "peerdiscoverer.h"

#include <QDebug>

PeerDiscoverer::PeerDiscoverer()
	: m_zeroconf(new QZeroConf) {

//	connect(m_zeroconf, SIGNAL(serviceUpdated(QZeroConfService)),
//		this, SLOT(serviceUpdated(QZeroConfService)));
//	connect(m_zeroconf, SIGNAL(serviceAdded(QZeroConfService)),
//		this, SLOT(serviceUpdated(QZeroConfService)));

	connect(m_zeroconf, SIGNAL(error(QZeroConf::error_t)), this, SLOT(printError(QZeroConf::error_t)));
	
	//m_zeroconf->startBrowser("_tcp.jsep", QAbstractSocket::IPv6Protocol);
}


void PeerDiscoverer::printError(QZeroConf::error_t error) {
	qWarning() << "QZC Error: " << error;
}
