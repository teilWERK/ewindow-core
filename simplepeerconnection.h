#include <QObject>
#include <QSharedPointer>

#include <qwebrtcsessiondescription.hpp>
#include <qwebrtcmediastream.hpp>
#include <qwebrtcicecandidate.hpp>

class QWebRTCPeerConnection;

// QML friendly wrapper of QWebRTCConnection

class SimplePeerConnection : public QObject {
    Q_OBJECT

public:
    SimplePeerConnection();
    ~SimplePeerConnection();

public Q_SLOTS:

    QSharedPointer<QWebRTCSessionDescription> createOffer();
    QSharedPointer<QWebRTCSessionDescription> createAnswer();

    bool setLocalDescription(QSharedPointer<QWebRTCSessionDescription>);
    bool setRemoteDescription(QSharedPointer<QWebRTCSessionDescription>);

    void addIceCandidate(const QSharedPointer<QWebRTCIceCandidate>& candidate);

private Q_SLOTS:
    void mediaStreamAdded(const QSharedPointer<QWebRTCMediaStream>&);
    void gotIceCandidate(const QSharedPointer<QWebRTCIceCandidate>&);

Q_SIGNALS:
    void videoTrackAdded(QObject* track);
    void iceCandidate(const QSharedPointer<QWebRTCIceCandidate>& candidate);

private:
    QSharedPointer<QWebRTCPeerConnection> m_pc;
};

Q_DECLARE_METATYPE(QSharedPointer<QWebRTCSessionDescription>)
