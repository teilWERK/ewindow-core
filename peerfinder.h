#pragma once

#include "qzeroconf.h"

/** Think of useful presence status, besides Available and Busy/InConnection
enum class PresenceStatus {
	
};
* */

class Peer : public QObject {
	Q_GADGET

	friend class PeerFinder;

public:
    Q_PROPERTY(QString name MEMBER name);
    Q_PROPERTY(int status MEMBER status);

private:
	QString name;
	QHostAddress ip;
	uint32_t interfaceIndex;
	int status;
};

class PeerFinder : public QObject {
	Q_OBJECT
public:
	PeerFinder();
	~PeerFinder();

	QList<Peer*> peers();

public Q_SLOTS:
	void setStatus(int i);

	void publish(QString hostname, QString service = "_qtzeroconf_test._tcp", uint16_t port=11437);
	void unpublish();


private:
	QZeroConf *m_zeroconf;
	QList<Peer*> m_peers;

	static QString make_key(QZeroConfService& service);
	static QString make_key(Peer* service);

private Q_SLOTS:
	void printError(QZeroConf::error_t);

	void peerAdded(QZeroConfService service);
	void peerUpdated(QZeroConfService service);
	void peerRemoved(QZeroConfService service);
};
