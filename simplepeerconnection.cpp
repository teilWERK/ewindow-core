#include "simplepeerconnection.h"

#include <qwebrtcpeerconnection.hpp>
#include <qwebrtcpeerconnectionfactory.hpp>
#include <qwebrtcmediastream.hpp>
#include <qwebrtcconfiguration.hpp>

#include <QDebug>
#include <QCoreApplication>


SimplePeerConnection::SimplePeerConnection()
{
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

    m_pc = pc;
}

SimplePeerConnection::~SimplePeerConnection()
{
}

QSharedPointer<QWebRTCSessionDescription> SimplePeerConnection::createOffer()
{
    // Fake createOffer to start ICE gathering..
    m_pc->createOfferForConstraints(QVariantMap(), [] (auto offer){
        /*qWarning() << "SDP fake offer";
        pc->setLocalDescription(offer, [](bool b){
            qWarning() << "SDP fake local description set";
        });*/
    });

    while (m_pc->iceGatheringState() != QWebRTCPeerConnection::IceGatheringState::Complete) {
        QCoreApplication::processEvents();
    }
    QSharedPointer<QWebRTCSessionDescription> sdp;
    auto sdpprint = [this, &sdp](auto sdp_) {
	//Q_EMIT(this->gotOfferSDP(sdp));
	sdp = sdp_;
    };
    
    auto varmap = QVariantMap();
    varmap["receiveAudio"] = true;
    varmap["receiveVideo"] = true;
    m_pc->createOfferForConstraints(varmap, sdpprint);

    while (!sdp);
    return sdp;
}


QSharedPointer<QWebRTCSessionDescription> SimplePeerConnection::createAnswer()
{
    // Fake createAnswer to start ICE gathering..
    m_pc->createAnswerForConstraints(QVariantMap(), [] (auto offer){
        /*qWarning() << "SDP fake offer";
        pc->setLocalDescription(offer, [](bool b){
            qWarning() << "SDP fake local description set";
        });*/
    });

    while (m_pc->iceGatheringState() != QWebRTCPeerConnection::IceGatheringState::Complete) {
        QCoreApplication::processEvents();
    }

    QSharedPointer<QWebRTCSessionDescription> sdp;
    auto sdpprint = [this, &sdp](auto sdp_) {
	//Q_EMIT(this->gotOfferSDP(sdp));
	sdp = sdp_;
    };
    
    auto varmap = QVariantMap();
    varmap["receiveAudio"] = true;
    varmap["receiveVideo"] = true;
    m_pc->createOfferForConstraints(varmap, sdpprint);

    while (!sdp);
    return sdp;
}



bool SimplePeerConnection::setLocalDescription(QSharedPointer<QWebRTCSessionDescription> sdp)
{
    bool waiting = true;
    int result;
    auto setlocal_callback = [&waiting, &result] (bool _result) {
        qWarning() << "pc->setRemoteDescription: " << result;
        waiting = false;
        result = _result;
    };

    m_pc->setLocalDescription(sdp, setlocal_callback);
    while (waiting);

    return result;
}

bool SimplePeerConnection::setRemoteDescription(QSharedPointer<QWebRTCSessionDescription> sdp)
{
    bool waiting = true;
    int result;
    auto remotedesc_callback = [&waiting, &result] (bool _result) {
        qWarning() << "pc->setRemoteDescription: " << result;
	waiting = false;
	result = _result;
    };

    m_pc->setRemoteDescription(sdp, remotedesc_callback);

    while (waiting);
    return result;
}
