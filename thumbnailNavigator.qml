import QtQuick 1.1

Rectangle {
    id: root

    width: 800
    height: 200

    color: "#808080"

    ListView {
        id: thumbnailNavigator
        objectName: "thumbnailNavigator"

        cacheBuffer: 400

        anchors.fill: parent

        orientation: ListView.Horizontal

        maximumFlickVelocity: 2000
        flickDeceleration: 2000

        spacing: 8

        model: thumbnailViewModel
        delegate: ThumbnailDelegate {}

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        highlightMoveDuration: 300
    }

    focus: true

    Keys.onLeftPressed: thumbnailNavigator.decrementCurrentIndex()
    Keys.onRightPressed: thumbnailNavigator.incrementCurrentIndex()
}
