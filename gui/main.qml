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

/*
    QWebRTCQuickVideoItem {
        id: videoItem
        anchors.fill: parent   
    }
    * */

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
    }

    /*
    PeerConnection {
        id: pcb
    }
    */

    Component.onCompleted: {
        var offer = pca.createOffer();
        console.log(offer);
        /*
        pca.setLocalDescription(offer);

        pcb.setRemoteDescription(offer);
        var answer = pcb.setRemoteDescription();
        console.log(answer);

        pca.setRemoteDescription(answer);
        */
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
