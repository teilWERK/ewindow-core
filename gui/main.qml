import QtQuick 2.7
import QtQuick.Window 2.2

import org.ewindow 0.1
import org.ewindow.ua 0.1

import "logic.js" as Logic

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480

	// A QML Object to attach new video renderers to
	// TODO: Make a renderer class that is filled by baresip_vidisp
    Rectangle {
        id: videoContainer
        anchors.fill: parent
    }    

	// Hook up newVideo events to the video container
    Connections {
        target: baresipCore
        onNewVideo: {
			console.info("Attaching new video device")
            video.parent = videoContainer
            video.anchors.fill = videoContainer
        }
    }

    FocusScope {
        focus: true
        width: parent.width
        height: parent.height
        //opacity: 0

        TimeoutDialog {
            id: contactListDialog

            focus: true
            onNext: contactList.selectNext()

            onTimeout: {
                //keyFocusItem.focus = true
                // hide()
            }

            onAction: {
                var uri = contactList.getCurrentURI()
                if (!uri) {
                    console.error("Fix the contactlist select bug")
                    return
                }

                console.info("selection complete, connecting to ", uri)
                Logic.make_connection(uri)
                hide()
                opacity = 0
            }

            ContactList {
                id: contactList
                model: contactListModel
                visible: true
            }
        }
/*
        TimeoutDialog {
            id: callConfirmation

            property string dname: "null"
            property string uri

            Text {
                font.pointSize: 36
                text: " Do you really want to call %s ?\n Press Button to confirm".arg(
                          "foo")
                //parent.dname)
            }

            onAction: {
                // hide() // Which Function is this? doesn't even exist
                console.warn("Not connecting to", uri)
                //ua.connect(uri)
            }
        }
*/
        Keys.onEscapePressed: {
            Qt.quit()
        }
        

        Keys.onPressed: {
                console.info("Window Keys.onPressed")
                contactListDialog.show()
        }

        MouseArea {
            width: parent.width
            height: parent.height

            //var volume = 100;
            property real volume: 100
            property real step: 5
            
            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    volume = Math.min(100, volume + step);
                }
                else if (wheel.angleDelta.y < 0) {
                    volume = Math.max(0, volume - step);
                }
                
                console.info("Wheel %f", wheel.angleDelta.y, volume);
                VolumeManager.setVolume(volume)
                fadeout.start()
            }

            Rectangle {
                color: "blue"
                height: parent.height / 10
                width: parent.width * 0.8 * parent.volume / 100
                anchors.centerIn: parent
            }

            NumberAnimation on opacity {
                id: fadeout
                from: 1.0
                to: 0.0
                easing.type: Easing.InExpo
                duration: 2345
            }
        }
    }
}
