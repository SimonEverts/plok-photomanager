import QtQuick 1.1

Rectangle {
    id: root

    width: 800
    height: 600

    color: "#808080"

    Column {

        anchors.fill: parent

        Image {
            width: parent.width
            height: parent.height - thumbnailNavigator.height - 5

            id: previewImage
            objectName: "previewImage"

            fillMode: Image.PreserveAspectFit

            sourceSize.width: 1920
        }

        ListView {
            id: thumbnailNavigator
            objectName: "thumbnailNavigator"

            cacheBuffer: 400

            width: parent.width
            height: 130
            anchors.top: previewImage.bottom
            //anchors.fill: parent

            orientation: ListView.Horizontal

            maximumFlickVelocity: 2000
            flickDeceleration: 2000

            spacing: 8

            model: thumbnailViewModel
            delegate: ThumbnailDelegate {}

            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            highlightMoveDuration: 300

            onCurrentIndexChanged: {
                previewImage.source = thumbnailViewModel[currentIndex].path;
                //loadNewImage( currentIndex );
            }

            signal loadNewImage (int index);
        }

        focus: true

        Keys.onLeftPressed: thumbnailNavigator.decrementCurrentIndex()
        Keys.onRightPressed: thumbnailNavigator.incrementCurrentIndex()
    }
}
