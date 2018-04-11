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

    PeerDiscoverer {
        id: zc
    }

    Component.onCompleted: {
        zc.publish()
    }

    Timer {
        interval: 1000
        running: true
        repeat: true

        property var counter: 0


        onTriggered: {
            console.log("foo", counter)
            switch (counter++) {
                case 0:
                    zc.stopPublish()
                    break;
                case 1:
                    zc.publish()
                    break;
                case 2:
                    zc.stopPublish()
                    break;
                case 3:
                    zc.publish();
                    break;
                case 3:
                    zc.setStatus(3);
                    break;
            }
        }
    }

/*
    QWebRTCPeerConnectionFactory {
        id: pcf
    }
    */
/*
    QWebRTCConfiguration {
        id: foo
    }
*/    

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
