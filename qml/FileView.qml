import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.1

Item {
    id: root

    property alias fileUrls: myModel

    function addFile(fileName) {
        myModel.append({ fileName : fileName });
    }

    function cleanup() {
        myModel.clear();
    }

    Component {
        id: delegate

        Rectangle {
            width: 60
            height: 60
            radius: 2
            clip: true
            color: hovered ? "#AAA" : "transparent";
            property bool hovered: false;

            Image {
                id: image
                width: 32
                height: 32
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                source: fileApi.fileToIcon(fileName)
            }

            Text {
                id: name
                width: parent.width - 4
                anchors.top:image.bottom
                anchors.topMargin: 8
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: fileApi.fileName(fileName)
                clip: true
                horizontalAlignment: contentWidth < width ? Text.AlignHCenter : Text.AlignLeft;
                elide: Text.ElideRight
            }

            MyToolTip {
                visible: parent.hovered
                text: name.text
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.hovered = true;
                onExited: parent.hovered = false;
                onClicked: myModel.remove(index);
            }
        }
    }

    GridView {
        id: gridView
        clip: true
        focus: true
        cellWidth: 65
        cellHeight: 65
        anchors.fill: parent
        anchors.margins: 8
        displaced: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
        }
        model: ListModel {
            id: myModel
        }
        delegate: delegate
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    }
}
