// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    Component {
        id: setViewDelegate
        Item {
            height: 16
            width: parent.width

            Text {
                id: name_label
                text: name
            }

            MouseArea {
                id: setViewDelegate_mousearea
                anchors.fill: parent
                onClicked: {
                    setView.currentIndex = index;
                }
            }
        }
    }

    ListView {
        width: parent.width

        id: setView
        keyNavigationWraps: false
        highlightFollowsCurrentItem: true
        interactive: true
        objectName: "setView"

        anchors.fill: parent
        anchors.margins: 5

        spacing: 5

        model: setViewModel
        delegate: setViewDelegate

        highlight: Rectangle {
            width: parent.width;
            color: "lightsteelblue";
        }

        focus: true
    }
}
