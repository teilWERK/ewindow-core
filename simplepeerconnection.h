#include <QObject>
#include <QSharedPointer>

#include <qwebrtcsessiondescription.hpp>

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

Q_SIGNALS:
    void gotOfferSDP(QSharedPointer<QWebRTCSessionDescription>);
    void gotAnswerSDP(QSharedPointer<QWebRTCSessionDescription>);

private:
    QSharedPointer<QWebRTCPeerConnection> m_pc;
};

Q_DECLARE_METATYPE(QSharedPointer<QWebRTCSessionDescription>)
