#include "simplepeerconnection.h"

#include <qwebrtcpeerconnection.hpp>
#include <qwebrtcpeerconnectionfactory.hpp>
#include <qwebrtcmediastream.hpp>
#include <qwebrtcmediatrack.hpp>
#include <qwebrtcconfiguration.hpp>

#include <QDebug>
#include <QCoreApplication>

#include <unistd.h>


SimplePeerConnection::SimplePeerConnection()
{
    m_pc_factory = QSharedPointer<QWebRTCPeerConnectionFactory>(new QWebRTCPeerConnectionFactory);
    
    QSharedPointer<QWebRTCMediaTrack> vt = m_pc_factory->createVideoTrack(QVariantMap(), "myvideo");
    QSharedPointer<QWebRTCMediaTrack> at = m_pc_factory->createAudioTrack(QVariantMap(), "myaudio");
    m_mediaStream = m_pc_factory->createMediaStream("mystream");
    
    m_mediaStream->addTrack(at);
    if (vt)
        m_mediaStream->addTrack(vt);
}

void SimplePeerConnection::createPeerConnection()
{
    QWebRTCConfiguration rtcconf;

    m_pc = m_pc_factory->createPeerConnection(rtcconf);
    m_pc->addStream(m_mediaStream);
    connect(m_pc.data(), &QWebRTCPeerConnection::streamAdded, this, &SimplePeerConnection::mediaStreamAdded);
    //connect(pc.data(), &QWebRTCPeerConnection::newIceCandidate, this, &SimplePeerConnection::gotIceCandidate);
}

SimplePeerConnection::~SimplePeerConnection()
{
    // Ensure correct de-initialization order
    m_pc.clear();
    m_mediaStream.clear();
    m_pc_factory.clear();
}

void SimplePeerConnection::mediaStreamAdded(const QSharedPointer<QWebRTCMediaStream>& stream)
{
    qWarning() << "SimplePeerConnection: new media stream" << stream;

    for (auto track : stream->tracks()) {
        if (track->trackType() == QWebRTCMediaTrack::Type::Video) {
            Q_EMIT videoTrackAdded(track.data());
            return;
        }
    }
}

QSharedPointer<QWebRTCSessionDescription> SimplePeerConnection::createOffer()
{
    if (!m_pc) createPeerConnection();

    // Fake createOffer to start ICE gathering..
    m_pc->createOfferForConstraints(QVariantMap(), [this] (auto offer){
        qWarning() << "SDP fake offer";
        m_pc->setLocalDescription(offer, [](bool b){
            qWarning() << "SDP fake local description set" << b;
        });
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

    while (!sdp); // TODO: Handle errors in SDP creation

    return sdp;
}


QSharedPointer<QWebRTCSessionDescription> SimplePeerConnection::createAnswer()
{
    if (!m_pc) createPeerConnection();
    /*
    // Fake createAnswer to start ICE gathering..
    m_pc->createAnswerForConstraints(QVariantMap(), [this] (auto answer){
        qWarning() << "SDP fake answer";
        m_pc->setLocalDescription(answer, [](bool b){
            qWarning() << "SDP fake local description set" << b;
        });
    });

    while (m_pc->iceGatheringState() != QWebRTCPeerConnection::IceGatheringState::Complete) {
        QCoreApplication::processEvents();
    }
    * */

    QSharedPointer<QWebRTCSessionDescription> sdp;
    auto sdpprint = [this, &sdp](auto sdp_) {
	//Q_EMIT(this->gotOfferSDP(sdp));
	sdp = sdp_;
    };
    
    auto varmap = QVariantMap();
    varmap["receiveAudio"] = true;
    varmap["receiveVideo"] = true;
    m_pc->createAnswerForConstraints(varmap, sdpprint);

    while (!sdp);
    return sdp;
}



bool SimplePeerConnection::setLocalDescription(QSharedPointer<QWebRTCSessionDescription> sdp)
{
    if (!m_pc) createPeerConnection();

    bool waiting = true;
    int result;
    auto setlocal_callback = [&waiting, &result] (bool _result) {
        qWarning() << "pc->setLocalDescription: " << result;
        waiting = false;
        result = _result;
    };

    m_pc->setLocalDescription(sdp, setlocal_callback);
    while (waiting);

    return result;
}

bool SimplePeerConnection::setRemoteDescription(QSharedPointer<QWebRTCSessionDescription> sdp)
{
    if (!m_pc) createPeerConnection();

    bool waiting = true;
    int result;
    auto remotedesc_callback = [&waiting, &result] (bool _result) {
        qWarning() << "pc->setRemoteDescription: " << _result;
	waiting = false;
	result = _result;
    };

    m_pc->setRemoteDescription(sdp, remotedesc_callback);

    while (waiting);
    return result;
}

void SimplePeerConnection::addIceCandidate(const QSharedPointer<QWebRTCIceCandidate>& candidate)
{
    m_pc->addIceCandidate(candidate);
}

void SimplePeerConnection::gotIceCandidate(const QSharedPointer<QWebRTCIceCandidate>& candidate)
{
    usleep(1);
    Q_EMIT(iceCandidate(candidate));
}

void SimplePeerConnection::close()
{
    m_pc->close();
    m_pc.clear();
}
