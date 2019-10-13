import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import an.file 1.0

Item {
    id: root

    property alias model: delegateModel.model

    Component {
        id: delegate

        Rectangle {
            id: wrapper
            width: listView.width - 12
            height: 64
            radius: 2
            clip: true
            border.color: "black"
            property bool complete: false;
            property int itemIndex: DelegateModel.itemsIndex;

            MouseArea {
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                anchors.fill: parent
                onClicked: {
                    //当右键点击并且已经传输完成时才可删除
                    if (mouse.button === Qt.RightButton && wrapper.complete)
                        delegateModel.items.remove(wrapper.itemIndex);
                }
            }

            Text {
                id: number
                color: "red"
                text: "[" + (wrapper.itemIndex + 1) + "]"
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
            }

            Text {
                id: name
                clip: true
                height: 16
                width: parent.width - 8
                anchors.top: number.bottom
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: contentWidth < width ? Text.AlignHCenter : Text.AlignLeft;
                elide: Text.ElideLeft
            }

            Rectangle {
                id: progressBar
                radius: 4
                clip: true
                border.color: "gray"
                anchors.top: name.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                height: 12
                width: parent.width - 8
                property real oldRatio: 0;
                property real ratio: offset / fileSize;
                onRatioChanged: {
                    name.text = fileName + " (" + fileApi.convertByte((ratio - oldRatio) * fileSize)
                              + "/s)"
                    animation.to = progressBar.ratio * progressBar.width;
                    animation.restart();
                    if (offset == fileSize) {
                        wrapper.complete = true;
                        progressRect.width = Qt.binding(function(){ return progressBar.width });
                        progressText.text += " [完成]";
                    }
                    oldRatio = ratio;
                }

                NumberAnimation {
                    id: animation
                    running: false
                    target: progressRect
                    duration: 1000
                    property: "width"
                    easing.type: Easing.Linear
                }

                Rectangle {
                    id: progressRect
                    radius: 4
                    height: parent.height
                    width: 0
                    color: "#C4D6FA" 
                }
            }

            Text {
                id: progressText
                clip: true
                height: 16
                width: parent.width - 8
                anchors.top: progressBar.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: contentWidth < width ? Text.AlignHCenter : Text.AlignLeft;
                elide: Text.ElideRight
                text: fileApi.convertByte(offset) + " / " + fileApi.convertByte(fileSize)
            }
        }
    }

    ListView {
        id: listView
        clip: true
        focus: true
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.bottomMargin: 14
        anchors.leftMargin: 8
        anchors.rightMargin: 2
        spacing: 10
        displaced: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
        }
        model: DelegateModel {
            id: delegateModel
            delegate: delegate
        }
        ScrollBar.vertical: ScrollBar {
            width: 10
            policy: ScrollBar.AsNeeded
        }
        onModelChanged: {
            positionViewAtEnd();
        }
    }
}
