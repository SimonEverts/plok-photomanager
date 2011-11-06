import QtQuick 1.1

Rectangle {
    width: 800
    height: 600

    GridView {
        id: thumbnailView
        cacheBuffer: 400

        anchors.fill: parent
        flow: GridView.LeftToRight

        cellHeight: 130
        cellWidth: 160


        delegate: Item {

            height: 100

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
        }

        model: thumbnailViewModel
    }
}
