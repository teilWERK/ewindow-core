import QtQuick 2.7
import QtQuick.Window 2.2

import org.ewindow 0.1
//import org.ewindow.webrtc 0.1
//import org.ewindow.ua 0.1

//import "logic.js" as Logic

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
    }

    CoolSocket {
        id: cs
    }

    PeerConnection {
        id: pca

        onIceCandidate: pcb.addIceCandidate(candidate)
    }

    
    PeerConnection {
        id: pcb
        
        onIceCandidate: pca.addIceCandidate(candidate)

        onVideoTrackAdded: videoItem.videoTrack = track
    }
    

    Component.onCompleted: {
        //pca.videoTrackAdded.connect(videoItem.setVideoTrack)
        console.log("!!!QML!!! pca.createOffer")
        var offer = pca.createOffer();
        console.log(offer);
        
        console.log("!!!QML!!! pca.setLocal", offer)
        pca.setLocalDescription(offer);
        
        console.log("!!!QML!!! pcb.setRemote", offer)
        pcb.setRemoteDescription(offer);
        
        console.log("!!!QML!!! pcb.createAnswer")
        var answer = pcb.createAnswer();
        console.log("!!!QML!!! pcb.setLocal", offer)
        pcb.setLocalDescription(answer);

        console.log("!!!QML!!! pca.setRemote", answer)
        pca.setRemoteDescription(answer);
        
        /*
        console.log("Determined port: ", typeof(cs.listenPort), cs.listenPort)
        zc.publish("ewindow-" + Math.random(), cs.listenPort)

        cl.contactSelected.connect(cs.connectTo)
        * */
    }

    Timer {
        interval: 5000
        running: true
        repeat: true

        property var counter: 0

        onTriggered: {
            zc.setStatus(counter++);
        }
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
