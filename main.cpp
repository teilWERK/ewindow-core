//#include <rtc_base/thread.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QSharedPointer>

#include <QDebug>

#include <QNetworkConfigurationManager>
#include <QTcpServer>

/*
#include <qwebrtcconfiguration.hpp>
#include <qwebrtcpeerconnectionfactory.hpp>
#include <qwebrtcpeerconnection.hpp>
#include <qwebrtcquickvideoitem.hpp>
#include <qwebrtcmediastream.hpp>
#include <qwebrtcmediatrack.hpp>


#include "baresipcore.h"
#include "baresipvidisp.h"
*/

#include "peerfinder.h"
#include "coolsocket.h"
#include "simplepeerconnection.h"


void registerTypes() {
	qRegisterMetaType<quint16>();
	qRegisterMetaType<QHostAddress>();
	
	qmlRegisterType<PeerFinder>("org.ewindow", 0, 1, "PeerFinder");
	qmlRegisterType<Peer>("org.ewindow", 0, 1, "Peer");
	
	qmlRegisterType<CoolSocket>("org.ewindow", 0, 1, "CoolSocket");

	qmlRegisterType<SimplePeerConnection>("org.ewindow", 0, 1, "PeerConnection");
	
	qRegisterMetaType<QSharedPointer<QWebRTCSessionDescription>>();
}

int main(int argc, char *argv[])
{
	// Try EGLFS when X server not present
	if (!getenv("DISPLAY")) {
		setenv("QT_QPA_PLATFORM", "eglfs", 0);
	}

	QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
	QGuiApplication app(argc, argv);

	app.setApplicationName("E - W I N D O W");
	app.setApplicationVersion("0.1");

	QQmlApplicationEngine engine;

	registerTypes();

	engine.load(QUrl(QStringLiteral("gui/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
