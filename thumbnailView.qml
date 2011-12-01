import QtQuick 1.1

Rectangle {
    width: 800
    height: 600

    color: "darkgrey"

    GridView {
        id: thumbnailView
        objectName: "thumbnailView"

        cacheBuffer: 400

        anchors.fill: parent
        flow: GridView.LeftToRight

        cellHeight: 140
        cellWidth: 170

        maximumFlickVelocity: 2000
        flickDeceleration: 2000

        model: thumbnailViewModel
        delegate: ThumbnailDelegate {}

        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        highlightMoveDuration: 300
    }
}
