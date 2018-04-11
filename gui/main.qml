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

    Component.onCompleted: {
        zc.publish("myfancyhostname")
    }

    Timer {
        interval: 800
        running: true
        repeat: true

        property var counter: 0


        onTriggered: {
            zc.setStatus(counter++);
        }
    }

    ContactList {
        model: zc.peers
        visible: true
        opacity: 0.5
    }

    Rectangle {
        color: 'red'
        width: 100
        height: 100
    }

    FocusScope {
        focus: true
        width: parent.width
        height: parent.height
        //opacity: 0
        
        Keys.onEscapePressed: {
            Qt.quit()
        }    

        Keys.onPressed: {
            return;
            console.info("Window Keys.onPressed")

            var ms = pcf.createMediaStream("my_media_stream")
            var vt = pcf.createVideoTrack({})

            ms.addTrack = vt
            console.log(ms)
            //videoItem.videoTrack = vt
            var pc = pcf.createPeerConnection(foo)
            pc.addStream(ms)

            var sdp = pc.createOffer()

            console.info(sdp)
            
            //var ms = pcf.createMediaStream("my_fancy_mediastream")
            //console.info(ms)
                //pc = pcf.createPeerConnection()
        }
    }
}
