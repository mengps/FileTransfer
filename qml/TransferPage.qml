import QtQuick 2.12
import QtQuick.Dialogs 1.3

Item
{

    Column
    {
        height: 400
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.right: separator1.left
        anchors.rightMargin: 50
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20
        clip: true

        Item
        {
            width: parent.width
            height: 300

            FileDialog
            {
                id: fileDialog
                title: "选择文件"
                folder: shortcuts.desktop
                nameFilters: ["All files (*)"]
                selectMultiple: true
                onAccepted:
                {
                    for(var i = 0; i < fileUrls.length; i++)
                    {
                        console.log(fileUrls[i]);
                        fileView.addFile(fileUrls[i]);
                    }
                }
            }

            Text
            {
                id: sendText
                anchors.verticalCenter: openFile.verticalCenter
                anchors.left: parent.left
                text: qsTr("放入需要发送的文件(可拖入)")
            }

            MyButton
            {
                id: openFile
                text: "选择文件"
                hoverColor: "#F5D2B5"
                anchors.left: sendText.right
                anchors.leftMargin: 8
                onClicked: fileDialog.open();
            }

            Rectangle
            {
                width: parent.width
                height: 250
                anchors.top: sendText.bottom
                anchors.topMargin: 15
                anchors.left: parent.left
                border.color: "black"
                radius: 5

                DropArea
                {
                    anchors.fill: parent;
                    onDropped:
                    {
                        if(drop.hasUrls)
                        {
                            for(var i = 0; i < drop.urls.length; i++)
                            {
                                console.log(drop.urls[i]);
                                fileView.addFile(drop.urls[i]);
                            }
                        }
                    }
                }

                FileView
                {
                    id: fileView
                    anchors.fill: parent
                }
            }
        }

        Item
        {
            id: name
            width: parent.width
            height: 40

            Row
            {
                anchors.centerIn: parent
                width: deleteButton.width + deleteButton.width + 20
                height: deleteButton.height
                spacing: 20

                MyButton
                {
                    id: deleteButton
                    hoverColor: "#F5D2B5"
                    text: "全部清空"
                    onClicked: fileView.cleanup();
                }

                MyButton
                {
                    id: sendButton
                    hoverColor: "#F5D2B5"
                    text: "发送"
                    onClicked:
                    {
                        for (var i = 0; i < fileView.fileUrls.count; i++)
                            fileTransfer.sendFile(fileView.fileUrls.get(i).fileName);
                        fileView.cleanup();
                    }
                }
            }
        }
    }


    Rectangle
    {
        id: separator1
        width: 5
        height: parent.height
        x: 380
        color: "#DDD"
        property real minX: 200;
        property real maxX: root.width - 100;

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            property point startPoint: Qt.point(0, 0);
            property point fixedPont: Qt.point(0, 0);

            onEntered: cursorShape = Qt.SizeHorCursor;
            onExited: cursorShape = Qt.ArrowCursor;
            onPressed: startPoint = Qt.point(mouseX, mouseY);
            onPositionChanged:
            {
                if(pressed)
                {
                    var offsetX = mouse.x - startPoint.x;
                    if ((separator1.x + offsetX) >= separator1.minX && (separator1.x + offsetX) <= separator1.maxX)
                        separator1.x += offsetX;
                }
            }
        }
    }

    GlowRectangle
    {
        id: sendFileLabel
        z: -1
        radius: 5
        height: 28
        color: "#D7C2F9"
        glowColor: color
        anchors.left: separator2.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: -radius

        Text
        {
            anchors.centerIn: parent
            text: qsTr("发送文件")
        }
    }

    FileProgress
    {
        id: sendFileProgress
        model: fileManager.writeFiles
        anchors.left: separator2.left
        anchors.right: parent.right
        anchors.top: sendFileLabel.bottom
        anchors.topMargin: 2
        anchors.bottom: separator2.top
    }

    Rectangle
    {
        id: separator2
        height: 5
        anchors.left: separator1.right
        anchors.right: parent.right
        y: 240
        color: "#DDD"
        property real minY: 150;
        property real maxY: root.height - 100;

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            property point startPoint: Qt.point(0, 0);
            property point fixedPont: Qt.point(0, 0);

            onEntered: cursorShape = Qt.SizeVerCursor;
            onExited: cursorShape = Qt.ArrowCursor;
            onPressed: startPoint = Qt.point(mouseX, mouseY);
            onPositionChanged:
            {
                if(pressed)
                {
                    var offsetY = mouse.y - startPoint.y;
                    if ((separator2.y + offsetY) >= separator2.minY && (separator2.y + offsetY) <= separator2.maxY)
                        separator2.y += offsetY;
                }
            }
        }
    }

    GlowRectangle
    {
        id: receiveFileLabel
        z: -1
        radius: 5
        height: 28
        color: "#D7C2F9"
        glowColor: color
        anchors.left: separator2.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: separator2.bottom
        anchors.topMargin: -radius

        Text
        {
            anchors.centerIn: parent
            text: qsTr("接收文件")
        }
    }

    FileProgress
    {
        id: receiveFileProgress
        model: fileManager.readFiles
        anchors.left: separator2.left
        anchors.right: parent.right
        anchors.top: receiveFileLabel.bottom
        anchors.topMargin: 2
        anchors.bottom: parent.bottom
    }
}
