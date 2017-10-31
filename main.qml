import QtQuick 2.7
import QtQuick.Window 2.2

import org.ewindow 0.1
//import org.ewindow.phone 0.1 as PhoneBackend

Window {
	visible: true
	width: 640
	height: 480
    color: "blue"

    id: mainWindow

    Rectangle {
        id: videoContainer
        anchors.fill: parent
        x: 5
        y: 10
        width: 300
        height: 300
    }

    Connections {
        target: baresipCore
        onNewVideo: {
            console.info("NEW VIDEO! HOORAY!! \o/")
            video.parent = videoContainer
            video.anchors.parent = videoContainer
            /*
            video.x = 100
            video.y = 100
            video.width = 500
            video.height = 400
            */

            video.width = videoContainer.width
            video.height = videoContainer.height
        }
    }

    UserAgent {
        id: ua
        onRinging: { console.info("RINGING!!!", call) }
        onIncoming: {
            console.info("INCOMING!!!", call)
            console.info("accepting call...")
            ua.accept(call)

        }
        /*onConnected: {}
        onHold: {}
        onDisconnected: {}
        onInterrupted: {}
        */
    }


    /*
    Timer {
        interval: 2400
        running: true
        repeat: true

        property bool vl: false
        onTriggered: {
            console.info("timer triggered")
            //ua.connect("strfry@[fde1:c0fe::4]")
            //ua.connect("k-ot@[fde1:c0fe::c3d2]")

            if (!vl)
                ua.startVidloop();
            else ua.stopVidloop();

            vl = !vl
        }
    }
    */

    ContactList {
        model: contactListModel

        onContactSelected: {
           console.info("delegate ENTER PRESSED. connecting to uri", uri)
           ua.connect(uri)
        }
    }

    /*
    Component {
        id: highlight
        Rectangle {
            id: rooot
            width: 180; height: 20
            color: ListView.isCurrentItem ? "black" : "red"; radius: 5
            y: list.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }
    */

}
