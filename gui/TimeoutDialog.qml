import QtQuick 2.0

Rectangle {
    signal next
    signal action
    signal timeout

    property real progress: 0.0

    color: "grey"
    border.color: "lightgrey"

    border.width: 2
    width: parent.width - 100
    height: parent.height - 100

    anchors.centerIn: parent

    radius: 10

    visible: false
    clip: true

    Rectangle {
        id: progressBar
        width: parent.width * parent.progress
        height: 10
        color: "blue"
        radius: parent.radius
        focus: parent.visible

        Keys.onPressed: {
            event.accepted = true
            if (event.isAutoRepeat)
                return
            if (visible) {
                progressTimer.start()
            }

            fadeout.restart()
            fadeout.stop()
        }

        Keys.onReleased: {
            event.accepted = true
            if (event.isAutoRepeat)
                return
            progressTimer.stop()
            fadeout.start()
            next()
        }
    }

    NumberAnimation on opacity {
        id: fadeout
        from: 1.0
        to: 0.0
        easing.type: Easing.InExpo
        duration: 2345
    }

    NumberAnimation on progress {
        id: progressTimer
        from: 0.0
        to: 1.0
        duration: 1234
    }

    function show() {
        console.info("TimeoutDialog::show")
        visible = true
        progress = 0.0

        fadeout.start()
    }

    function hide() {
        visible = false
    }

    Component.onCompleted: {
        fadeout.onStopped.connect(function (foo) {
            if (opacity == 0.0) {
                visible = false
                timeout()
            }
        })

        progressTimer.onStopped.connect(function () {
            if (progress === 1.0) {
                action()
            } else {
                progress = 0.0
            }
        })
    }
}
