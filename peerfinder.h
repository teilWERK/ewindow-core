#pragma once

#include "qzeroconf.h"

#include <QQmlListProperty>

// We need to tell Qt that these exist to use them in QML
Q_DECLARE_METATYPE(QHostAddress)

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
	Q_PROPERTY(quint16 port MEMBER m_port NOTIFY portChanged);
	Q_PROPERTY(QString interface MEMBER m_interface NOTIFY interfaceChanged);

Q_SIGNALS:
	void nameChanged();
	void statusChanged();
	void ipChanged();
	void portChanged();
	void interfaceChanged();

private:
	QString m_name;
	int m_status;
	QHostAddress m_ip;
	quint16 m_port;
	QString m_interface;

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

	// port whould be within uint16_t
	void publish(QString hostname, int port, QString service = "_ewindow._tcp");
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
