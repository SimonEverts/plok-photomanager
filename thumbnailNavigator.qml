import QtQuick 1.1

Rectangle {
    id: root

    width: 800
    height: 200

    Component {
        id: thumbnailDelegate
        Item {
            width: 150
            height: 130

            Column {
                spacing: 5
                Image {
                    width: 150
                    height: 100
                    source: path
                    anchors.horizontalCenter: parent.horizontalCenter

                    asynchronous: true
                    sourceSize.width: 150
                    sourceSize.height: 100
                }

                Text {
                    text: name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                }
            }

            MouseArea {
                id: thumbnail_mousearea
                anchors.fill: parent
                onClicked: {
                    thumbnailNavigator.currentIndex = index

                }
            }
        }
    }

    ListView {
        id: thumbnailNavigator
        cacheBuffer: 400

        anchors.fill: parent

        orientation: ListView.Horizontal

        maximumFlickVelocity: 2000
        flickDeceleration: 2000

        spacing: 8

        model: thumbnailViewModel
        delegate: thumbnailDelegate

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }

        onCurrentIndexChanged: mainWindow.currentIndexChanged (currentIndex)
    }

    focus: true

    Keys.onLeftPressed: thumbnailNavigator.decrementCurrentIndex()
    Keys.onRightPressed: thumbnailNavigator.incrementCurrentIndex()
}
