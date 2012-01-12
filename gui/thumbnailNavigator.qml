import QtQuick 1.1

Rectangle {
    id: root

    width: 800
    height: 600

    color: "#808080"

    Column {

        anchors.fill: parent

        spacing: 4

        Column {
            id: imageFrame
            objectName: "imageFrame"

            property variant infoMap : {
                "iso_speed" : 0,
                        "aperture" : 0,
                        "shutter" : 0,
                        "focal_len" : 0,
                        "make" : "unknown",
                        "model" : "unknown",
                        "lens" : "unknown",
                        "timestamp" : "unknown"
            }

            width: parent.width;
            height: parent.height - thumbnailNavigator.height

            spacing: 2

            Image {
                width: parent.width
                height: parent.height - info.height - info_text.height

                cache: false
                asynchronous: false

                anchors.top: imageFrame.top

                id: previewImage
                objectName: "previewImage"

                fillMode: Image.PreserveAspectFit
                smooth: true
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

            Row {
                height: 13

                anchors.horizontalCenter: parent.horizontalCenter

                spacing: 10

                id: info_text

                Text {
                    font.pixelSize: 10

                    text: '<b>Name: </b>' + thumbnailViewModel[thumbnailNavigator.currentIndex].name
                }
                Text {
                    font.pixelSize: 10

                    text: '<b>Camera: </b>' + imageFrame.infoMap.make + " " + imageFrame.infoMap.model
                }
                Text {
                    font.pixelSize: 10

                    text: '<b>Lens: </b>' + imageFrame.infoMap.lens
                }
                Text {
                    font.pixelSize: 10

                    text: '<b>Date: </b>' + imageFrame.infoMap.timestamp.toString()
                }
            }
        }

        ListView {
            id: thumbnailNavigator
            objectName: "thumbnailNavigator"

            cacheBuffer: 1920

            width: parent.width
            height: 130

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
