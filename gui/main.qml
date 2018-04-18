import QtQuick 2.7
import QtQuick.Window 2.2

import org.ewindow 0.1

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480

    VideoItem {
        id: videoItem
        anchors.fill: parent   
    }    

    PeerFinder {
        id: zc
    }

    ContactList {
        id: cl
        model: zc.model

        visible: true
        opacity: 0.2
    }

    CoolSocket {
        id: cs

        onReceivedOffer: {
            console.log("QML!!! received offer")
            pca.setRemoteDescription(sdp);
            var sdp = pca.createAnswer();
            pca.setLocalDescription(sdp);
            console.log("QML!!! sending Answer ", sdp)
            sendAnswer(sdp);
        }

        onReceivedAnswer: {
            pca.setRemoteDescription(sdp)
        }
    }


    PeerConnection {
        id: pca

        //onIceCandidate: pcb.addIceCandidate(candidate)
        onVideoTrackAdded: videoItem.videoTrack = track
    }

    Component.onCompleted: {
        console.log("Determined port: ", typeof(cs.listenPort), cs.listenPort)
        zc.setStatus(1)
        zc.publish("ewindow-" + Math.floor(Math.random()* 10000), cs.listenPort)
        
        cl.contactSelected.connect(function (ip, port) {
            console.log("!!!QML!!! pca.createOffer")
            var offer = pca.createOffer();
            pca.setLocalDescription(offer);
            cs.connectTo(ip, port, offer);
            //cs.connectTo(ip, 5555, offer);
        })
        return 
        
    }


    FocusScope {
        focus: true
        
        Keys.onEscapePressed: {
            Qt.quit()
        }    

        Keys.onPressed: {
            return;
            console.info("Window Keys.onPressed")

            var pc = rtc.preparePeerConnection()
            var sdp = rtc.offerSDP()

            rtc.sendSDP(url); //???
        }
    }
}
