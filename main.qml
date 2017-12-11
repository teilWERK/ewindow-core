import QtQuick 2.7
import QtQuick.Window 2.2

import org.ewindow 0.1
//import org.ewindow.phone 0.1 as PhoneBackend

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480

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
            video.parent = videoContainer
            video.anchors.fill = videoContainer
            video.width = videoContainer.width
            video.height = videoContainer.height
        }
    }

    UserAgent {
        id: ua

        property var currentCall: null

        onCurrentCallChanged: {
            console.info(ContactListModel.PRESENCE_BUSY)
            setPresence(currentCall ? ContactListModel.PRESENCE_BUSY : ContactListModel.PRESENCE_OPEN)
        }

        onRinging: {
            console.info("RINGING!!!", call)
        }
        onIncoming: {
            console.info("INCOMING!!!", call)
            ua.accept(call)
            if (!currentCall) {
                console.info("accepting call...")
                ua.accept(call)
            } else {
                // call dialog
            }
        }

        onConnected: {
            console.info("CONNECTED!!!", call)
            currentCall = call
        }

        onDisconnected: {
            console.info("DISCONNECTED!!!", call)
            currentCall = null
        }

        onInterrupted: {
            console.info("INTERRUPTED?!?", call)
        }
    }

    TimeoutDialog {
        id: timeoutDialog
    }

    /// Timer for Vidloop testing
    Timer {
        interval: 5400

        //running: true
        repeat: true
        property bool vl: false
        onTriggered: {
            console.info("timer triggered")
            //ua.connect("strfry@[fde1:c0fe::4]")
            //ua.connect("k-ot@[fde1:c0fe::c3d2]")

            if (!vl)
                ua.startVidloop();
            else ua.stopVidloop();

            timeoutDialog.show()

            /*
            if (!vl)
                ua.startVidloop()
            else
                ua.stopVidloop()
                */
            vl = !vl
        }
    }
    
    TimeoutDialog {
        id: contactListDialog
        onNext: contactList.selectNext()

        onTimeout: {
            keyFocusItem.focus = true
            // hide()
        }

        onAction: {
            hide()
            var uri = contactList.getCurrentURI()
            if (!uri) {
                console.error("Fix the contactlist select bug")
                return
            }

            console.info("selection complete, connecting to ", uri)

            callConfirmation.dname = uri
            callConfirmation.uri = uri
            callConfirmation.show()
        }

        ContactList {
            id: contactList
            model: contactListModel
            visible: true
        }

        Keys.onPressed: {
            console.info("TimeoutDialog parent Keys.onPressed")
            show()
        }
    }

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

        onVisibleChanged: {
            if (!visible)
                keyFocusItem.visible = true
        }
    }

    Item {
        id: keyFocusItem
        focus: true

        Keys.onPressed: {
            console.info("Window Keys.onPressed")
            contactListDialog.show()
        }
    }
}
