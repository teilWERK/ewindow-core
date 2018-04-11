#pragma once

#include <qzeroconf.h>

class PeerDiscoverer : public QObject {
	Q_OBJECT
public:
	PeerDiscoverer();

public Q_SLOTS:
	void setStatus(int i) {
		/*
		stopPublish();
		m_zeroconf->clearServiceTxtRecords();
		m_zeroconf->addServiceTxtRecord("status", QString(i));
		m_zeroconf->startServicePublish("myname", "_tcp.jsep", "local", 5555);
		* */
	}

	void publish() {
		m_zeroconf->startServicePublish("myname", "_tcp.jsep", "local", 5555);
	}

	void stopPublish() {
		m_zeroconf->stopServicePublish();
	}

	void printError(QZeroConf::error_t);

private:
	QZeroConf *m_zeroconf;


private Q_SLOTS:
	void serviceUpdated(QZeroConfService service) {
		qInfo() << "Service Update" << service;
		for (auto x : service.txt()) {
			qInfo() << x;
		}
	}
};
