import QtQuick 2.7
//import QtQml.Models 2.2
import QtQuick.Window 2.2

import org.ewindow 0.1
//import org.ewindow.phone 0.1 as PhoneBackend

Window {
	visible: true
	width: 640
	height: 480
    color: "blue"

    Rectangle {
        id: videoContainer

        anchors.fill: parent

        //visible: true
    }

    Connections {
        target: baresipCore
        onNewVideo: {
            console.info("NEW VIDEO! HOORAY!! \o/")
            video.parent = videoContainer
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

    Timer {
        interval: 1400
        running: true
        repeat: false
        onTriggered: {
            console.info("timer triggered")
            //ua.connect("strfry@[fde1:c0fe::4]")
        }
    }

MouseArea
{
	anchors.fill: parent
	onClicked: {
        //if (!contactBox.visible) bth.start()
		contactBox.visible = !contactBox.visible

        console.log("clicked log")
        console.warn("clicked warn")
        console.error("clicked err")

	}
}

Rectangle {
	id: contactBox
        x: 100
        y: 10
         width: 180; height: 200
         clip: true
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
         Component {
             id: contactDelegate
             Item {
                 width: 180; height: 40
                 Column {
                     Text { text: '<b>Name:</b> ' + dname }
                     Text { text: '<b>URI:</b> ' + uri }
                     Text { text: '<b>Presence:</b> ' + presence }
                 }
                 MouseArea {
                    anchors.fill: parent
                    onClicked: listView.currentIndex = index
                }
             }
         }

         ListView {
             id: listView
             anchors.fill: parent
             //model: listModle
             model: contactListModel
             delegate: contactDelegate
             highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
             focus: true
             contentHeight: 150
         }
}

}
