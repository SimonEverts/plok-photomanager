// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    width: 100
    height: 600

    Component {
        id: setViewDelegate

        Item {
            height: 32
            width: parent.width

            Column {
                x: 5

                Text {
                    id: name_label
                    text: name
                }

                Text {
                    id: date_label
                    text: "Date: " + date;
                }
            }

            MouseArea {
                id: setViewDelegate_mousearea
                anchors.fill: parent
                onClicked: {
                    setViewList.currentIndex = index;
                }
            }
        }
    }

    Column {
        id: column1
        anchors.fill: parent

        ListView {
            id: setViewList
            objectName: "setViewList"

            width: parent.width
            height: parent.height - listButtons.height

            keyNavigationWraps: false
            highlightFollowsCurrentItem: true
            interactive: true


            //anchors.fill: parent
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

        Row {
            id: listButtons
            objectName: "listButtons"

            signal createSet;
            signal deleteSet;

            spacing: 4

            Rectangle {
                width: 32
                height: 32

                border.color: "lightsteelblue"
                border.width: 1

                Image {
                    anchors.fill: parent

                    source: "icons/22x22/list-add.png"
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        listButtons.createSet();
                        console.log ("createSet");
                    }
                }
            }
            Rectangle {
                width: 32
                height: 32

                border.color: "lightsteelblue"
                border.width: 1

                Image {
                    anchors.fill: parent

                    source: "icons/22x22/list-remove.png"
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        console.log ("createSet");
                        listButtons.deleteSet();
                    }
                }
            }
        }
    }
}
