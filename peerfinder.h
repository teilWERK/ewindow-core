#pragma once

#include "qzeroconf.h"

class Peer;
class PeerList;

class PeerFinder : public QObject {
	Q_OBJECT

	Q_PROPERTY(PeerList* model READ model NOTIFY peersChanged);

public:
	PeerFinder();
	~PeerFinder();

public Q_SLOTS:
	// Set the current presence status. TODO: Make a proper enum with states
	void setStatus(int i);

	// port whould be within uint16_t
	void publish(QString hostname, int port, QString service = "_ewindow._tcp");
	void unpublish();

	PeerList* model();

Q_SIGNALS:
	void peersChanged();

private:
	QZeroConf *m_zeroconf;
	PeerList* m_peerlist;

	static QString make_key(const QZeroConfService& service);
	static QString make_key(const Peer* service);

private Q_SLOTS:
	void printError(QZeroConf::error_t);
};
