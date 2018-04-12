#pragma once

#include "qzeroconf.h"

#include <QQmlListProperty>

class Peer : public QObject {
	Q_OBJECT
	/** Think of useful presence status, besides Available and Busy/InConnection
	enum class PresenceStatus {
		AVAILABLE = 0,
		BUSY,
		INCLOSEDCONNECTION,
	};
	*/

	Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged);
	Q_PROPERTY(int status MEMBER m_status NOTIFY statusChanged);
	Q_PROPERTY(QHostAddress ip MEMBER m_ip NOTIFY ipChanged);
	Q_PROPERTY(uint32_t interfaceIndex MEMBER m_interfaceIndex);

Q_SIGNALS:
	void nameChanged();
	void statusChanged();
	void ipChanged();

private:
	QString m_name;
	int m_status;
	QHostAddress m_ip;
	quint32 m_interfaceIndex;

	friend class PeerFinder;
};

class PeerFinder : public QObject {
	Q_OBJECT

	Q_PROPERTY(QQmlListProperty<Peer> model READ model NOTIFY peersChanged);

public:
	PeerFinder();
	~PeerFinder();

public Q_SLOTS:
	// Set the current presence status. TODO: Make a proper enum with states
	void setStatus(int i);

	void publish(QString hostname, QString service = "_ewindow._tcp", uint16_t port=11437);
	void unpublish();

	QQmlListProperty<Peer> model();

Q_SIGNALS:
	void peersChanged();

private:
	QZeroConf *m_zeroconf;
	QList<Peer*> m_peerlist;

	static QString make_key(const QZeroConfService& service);
	static QString make_key(const Peer* service);

private Q_SLOTS:
	void printError(QZeroConf::error_t);

	void peerAdded(QZeroConfService service);
	void peerUpdated(QZeroConfService service);
	void peerRemoved(QZeroConfService service);
};
