import QtQuick 1.1

Rectangle {
    width: 800
    height: 600

    color: "darkgrey"

    GridView {
        id: thumbnailView
        objectName: "thumbnailView"

        cacheBuffer: 400

        //: "darkgray"

        anchors.fill: parent
        flow: GridView.LeftToRight

        cellHeight: 140
        cellWidth: 170

        maximumFlickVelocity: 2000
        flickDeceleration: 500

        model: thumbnailViewModel
        delegate: ThumbnailDelegate {}

        highlight: Rectangle {
            border.width: 3
            border.color: "black"
            color: "lightsteelblue"; radius: 5 }
    }
}
