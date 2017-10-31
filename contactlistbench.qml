import QtQuick 2.0
import QtQuick.Window 2.3

import "."

Rectangle {
    width: 800
    height: 600

    color: "green"
    visible: true
    focus: true


    ContactList {
        id: contactList

        model: testModel
    }

    Keys.onSpacePressed: {
        contactList.visible = !contactList.visible
    }

    ListModel {
        id: testModel

        ListElement {
            logo: "cities/mantova.png"
            dname: "Multifactory Mantova"
            available: true
            presence: 1
        }


        ListElement {
            dname: "Milano"
            logo: "cities/milano.svg"
            available: false
            presence: 0
        }

        ListElement {
            dname: "RosenWERK Dresden"
            logo: "cities/dresden.svg"
            available: true
            presence: 1
        }
    }
}
