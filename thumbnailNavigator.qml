import QtQuick 1.1

Rectangle {
    id: root

    width: 800
    height: 600

    color: "#808080"

    Column {

        anchors.fill: parent

        Column {
            id: imageFrame
            objectName: "imageFrame"

            property variant infoMap: { 'iso_speed' : 0, 'aperture': 0, 'shutter' : 0, 'focal_len' : 0}

            width: parent.width;
            height: parent.height - thumbnailNavigator.height - 15

            spacing: 4

            Image {
                width: parent.width
                height: parent.height - info.height - 4

                anchors.top: imageFrame.top

                id: previewImage
                objectName: "previewImage"

                fillMode: Image.PreserveAspectFit

                sourceSize.width: 1920
            }

            Row {
                height: 16

                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 10

                id: info

                Text {
                    font.bold: true

                    text: 'ISO' + imageFrame.infoMap.iso_speed
                }
                Text {
                    font.bold: true

                    text: '<i>ƒ</i>/' + imageFrame.infoMap.aperture.toFixed(1)
                }
                Text {
                    font.bold: true

                    text: '1/' + (1/ imageFrame.infoMap.shutter).toFixed(0)
                }
                Text {
                    font.bold: true

                    text: imageFrame.infoMap.focal_len + 'mm'
                }
            }
        }

        ListView {
            id: thumbnailNavigator
            objectName: "thumbnailNavigator"

            cacheBuffer: 1920

            width: parent.width
            height: 130
            anchors.top: imageFrame.bottom
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
                //previewImage.source = "image://imageprovider/" + thumbnailViewModel[currentIndex].path;
                loadNewImage( currentIndex );
            }

            signal loadNewImage (int index);
        }

        focus: true

        Keys.onLeftPressed: thumbnailNavigator.decrementCurrentIndex()
        Keys.onRightPressed: thumbnailNavigator.incrementCurrentIndex()
    }
}
