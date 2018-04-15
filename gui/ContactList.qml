import QtQuick 2.7

import org.ewindow 0.1

ListView {
    id: listView

    anchors.fill: parent
    clip: true

    delegate: contactDelegate

    signal contactSelected(variant ip, int port)

    highlight: Rectangle {
        color: "blue"
        radius: 5
    }


    // Helper function for the old one-button-interface
    /*
    function selectNext() {
        var view = listView
        console.info(view.currentIndex, view.count)
        if (view.currentIndex + 1 >= view.count) {
            view.currentIndex = 0
        } else {
            view.currentIndex++
        }
    }
    */

    Component {
        id: contactDelegate
        Rectangle {
            border.width: 3
            height: 100
            width: parent.width
            clip: true

            MouseArea {
               anchors.fill: parent
               onClicked: {
                   if (index == listView.currentIndex) {
                       //contactSelected(model)
                       contactSelected(ip, port)
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

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                x: y
                width: parent.height / 4
                height: width
                radius: width

                color: status != 0 ? "green" : "red"
            }

            Row {
                anchors {
                    left: idLogo.right;
                    right: parent.right
                }

                Text {
                    //text: model.name

                    verticalAlignment: Text.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 100
                    fontSizeMode: Text.HorizontalFit
                }

                Text {
                    text: name + model.status + model.ip.toString() + model.port + model.interface
                }
            }
        }
    }

}
