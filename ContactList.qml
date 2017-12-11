import QtQuick 2.7
import QtGraphicalEffects 1.0

import org.ewindow 0.1

ListView {
    model: parent.model
    anchors.fill: parent

    id: listView

    clip: true

    delegate: contactDelegate

    highlight: Rectangle {
        color: "blue"
        radius: 5
    }

    function selectNext() {
        var view = listView
        console.info(view.currentIndex, view.count)
        if (view.currentIndex + 1 >= view.count) {
            view.currentIndex = 0
        } else {
            view.currentIndex++
        }
    }

    function getCurrentURI() {
        var row = listView.currentIndex
        if (row == -1)
            return null
        var idx = model.index(row, 0)
        var dat = model.data(idx, ContactListModel.URIRole)
        return dat
    }

    anchors.centerIn: parent

    OpacityAnimator on opacity {
        id: fadeIn
        from: 0
        to: .55
        duration: 333
        running: true
    }

    OpacityAnimator on opacity {
        id: fadeOut
        from: opacity
        to: 0
        duration: 333
        running: false
    }

    onVisibleChanged: {
        console.info("changed visible ->", visible)
        if (visible) {
            fadeIn.start()
        } else {
            fadeOut.start()
        }
    }

    Component {
        id: contactDelegate
        Rectangle {
            //color: "white"
            border.width: 3
            height: 100
            width: parent.width
            clip: true

            opacity: 1

            MouseArea {
               anchors.fill: parent
               onClicked: {
                   if (index == listView.currentIndex) {
                       contactSelected(uri)
                   }

                   listView.currentIndex = index
               }
           }

            Image {
                id: idLogo

                anchors.margins: 15
                //anchors.horizontalCenter: parent.horizontalCenter
                //anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                //anchors.horizontalCenter: parent
                //source: logo
                x: 30
                height: 90
                width: height
            }

            Colorize {
                anchors.fill: idLogo
                source: idLogo
                hue: 0
                lightness: 0
                saturation: 0

                //visible: !available
                visible: presence != 1
            }


            Text {
                text: dname

                anchors { left: idLogo.right;
                    right: parent.right
                }

                verticalAlignment: Text.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 100
                fontSizeMode: Text.HorizontalFit
            }
        }
    }

}
