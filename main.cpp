//#include <rtc_base/thread.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QSharedPointer>

#include <QDebug>

#include <unistd.h>
#include <libgen.h>

#include <cassert>

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

void registerTypes() {
	/*
	//qmlRegisterInterface<BaresipCore>("BaresipCore");
	//qmlRegisterSingletonType<UserAgent>("org.ewindow.ua", 0, 1, "UserAgent", &UserAgent::newInstance);
	//qmlRegisterSingletonType<UserAgent>("org.ewindow", 0, 1, "VolumeManager", &VolumeManager::newInstance);
	qmlRegisterUncreatableType<BaresipVidisp>("org.ewindow", 0, 1, "VideoDisplay", "VideoDisplay is created by the backend, use the onNewVideo callback");
	//qmlRegisterUncreatableType<ContactListModel>("org.ewindow", 0, 1, "ContactListModel", "ContactListModel is passed by the application through "contactListModel"");

	qmlRegisterInterface<QWebRTCMediaStream>("QWebRTCMediaStream");
	qmlRegisterInterface<QWebRTCMediaTrack>("QWebRTCMediaTrack");
	
	qRegisterMetaType<QSharedPointer<QWebRTCMediaTrack>>();
	qRegisterMetaType<QSharedPointer<QWebRTCPeerConnection>>();

	//qRegisterMetaType<QWebRTCConfiguration>();
	//qmlRegisterType<QWebRTCConfiguration>("org.ewindow.webrtc", 0, 1, "QWebRTCConfiguration");
	
	qmlRegisterInterface<QSharedPointer<QWebRTCMediaTrack>>("QWebRTCMediaTrackPtr");
	qmlRegisterInterface<QWebRTCPeerConnection>("QWebRTCPeerConnection");
	qmlRegisterInterface<QWebRTCIceServer>("QWebRTCIceServer");
	
	
	qmlRegisterType<QWebRTCPeerConnectionFactory>("org.ewindow.webrtc", 0, 1, "QWebRTCPeerConnectionFactory");
	qmlRegisterType<QWebRTCQuickVideoItem>("org.ewindow.webrtc", 0, 1, "QWebRTCQuickVideoItem");

	*/

	//qmlRegisterType<QZeroConf>("org.ewindow", 0, 1, "QZeroConf");
	qmlRegisterType<PeerFinder>("org.ewindow", 0, 1, "PeerFinder");
}


int main(int argc, char *argv[])
{
	// Try EGLFS when X server not present
	if (!getenv("DISPLAY")) {
		setenv("QT_QPA_PLATFORM", "eglfs", 0);
	}

	QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
	QGuiApplication app(argc, argv);

	
	app.setApplicationName("QtZeroConf Example");
	app.setApplicationVersion("0.1");

	QQmlApplicationEngine engine;

	registerTypes();

	engine.load(QUrl(QStringLiteral("gui/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}

#if 0

void doTheWebRTCDance() {
	QWebRTCPeerConnectionFactory pc_factory;
	QSharedPointer<QWebRTCMediaTrack> vt = pc_factory.createVideoTrack(QVariantMap(), "video");
	QSharedPointer<QWebRTCMediaTrack> at = pc_factory.createAudioTrack(QVariantMap(), "audio");
	
	QSharedPointer<QWebRTCMediaStream> ms = pc_factory.createMediaStream("mystream");
	ms->addTrack(at);
	ms->addTrack(vt);

	QWebRTCConfiguration rtcconf;

	QSharedPointer<QWebRTCPeerConnection> pc = pc_factory.createPeerConnection(rtcconf);
	pc->addStream(ms);
	//pc->addTrack(vt);

    auto remotedesc_callback = [] (bool result) {
        qWarning() << "pc->setRemoteDescription: " << result;
    };


    // Fake createOffer to start ICE gathering..
    pc->createOfferForConstraints(QVariantMap(), [pc] (auto offer){
        qWarning() << "SDP fake offer";
        pc->setLocalDescription(offer, [](bool b){
            qWarning() << "SDP fake local description set";
        });
    });

    while (pc->iceGatheringState() != QWebRTCPeerConnection::IceGatheringState::Complete) {
        app.processEvents();
    }


    auto sdpprint = [pc, remotedesc_callback] (auto offer) {
        qWarning() << "SDP offer:" << QString(offer->sdp());
        pc->setLocalDescription(offer, [pc, offer, remotedesc_callback] (bool success) {
            qWarning() << "pc->setLocalDescription: " << success;
            pc->setRemoteDescription(offer, remotedesc_callback);
        });
    };

	auto varmap = QVariantMap();
    varmap["receiveAudio"] = true;
    varmap["receiveVideo"] = true;
	pc->createOfferForConstraints(varmap, sdpprint);
#endif

