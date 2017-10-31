import QtQuick 2.7
import QtGraphicalEffects 1.0

Rectangle {
    border.color: "lightgrey"
    color: "grey"
    border.width: 2
    //opacity: 0.77

    width: parent.width - 100
    height: parent.height - 100

    anchors.centerIn: parent

    radius: 10

    signal contactSelected(string uri)

    //property model: null
    property var model: null;

    OpacityAnimator on opacity{
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
                source: logo
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

    ListView {
        model: parent.model
        id: listView
        anchors.fill: parent

        clip: true

        delegate: contactDelegate

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
    }

}


    /*
    Rectangle {

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

                                    Keys.onReturnPressed: {
                                        console.info("delegate ENTER PRESSED. connecting to uri", uri)
                                        ua.connect(uri)
                                    }
                                }
                            }

                   }

                   }
    */
