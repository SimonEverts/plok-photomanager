import QtQuick 1.1

Component {
    id: thumbnailDelegate
    Item {
        id: wrapper
        width: 160
        height: 130

        Rectangle {
            border.width: 1
            border.color: "black"

            anchors.fill: parent
            color: "lightgreen"
            radius: 5
            opacity: selected ? 1 : 0;
        }

        Column {
            x: 5
            y: 5
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
                mainWindow.currentSelectionChanged (index)
            }
            onDoubleClicked: {
                mainWindow.doubleClickOnThumbnail (index)
            }
        }
    }
}
