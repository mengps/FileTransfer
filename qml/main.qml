import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window
{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("File Transfer")
    color: "#D4E6FF"
    Component.onCompleted: flags |= Qt.FramelessWindowHint;

    ResizeMouseArea
    {
        anchors.fill: parent
        target: root
    }

    GlowRectangle
    {
        id: windowTitle
        radius: 5
        z: 5
        height: 40
        width: parent.width
        color: "#F4D1B4"
        glowColor: color
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: -radius

        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            text: qsTr("文件传输")
        }

        MyButton
        {
            heightMargin: 8
            anchors.right: parent.right
            anchors.top: parent.top
            text: "X"
            onClicked: root.close();
        }
    }

    Container
    {
        id: tabBar
        width: 50
        anchors.top: windowTitle.bottom
        anchors.bottom: parent.bottom

        contentItem: ListView
        {
            interactive: false
            model: tabBar.contentModel
            snapMode: ListView.SnapOneItem
            orientation: ListView.Vertical
        }

        Rectangle
        {
            width: tabBar.width
            height: tabBar.height / 2
            color:
            {
                if (pressed) return "#88AAAAAA"
                else if (tabBar.currentIndex == 0)
                    return "#88333333";
                else return "transparent";
            }
            property bool pressed: false;

            Text
            {
                anchors.centerIn: parent
                font.pointSize: 10
                text: "扫\n描"
            }

            MouseArea
            {
                anchors.fill: parent
                onPressed: parent.pressed = true;
                onReleased: parent.pressed = false;
                onClicked: tabBar.setCurrentIndex(0);
            }
        }

        Rectangle
        {
            width: tabBar.width
            height: tabBar.height / 2
            color:
            {
                if (pressed) return "#88AAAAAA"
                else if (tabBar.currentIndex == 1)
                    return "#88333333";
                else return "transparent";
            }
            property bool pressed: false;

            Text
            {
                anchors.centerIn: parent
                font.pointSize: 10
                text: "发\n送"
            }

            MouseArea
            {
                anchors.fill: parent
                onPressed: parent.pressed = true;
                onReleased: parent.pressed = false;
                onClicked: tabBar.setCurrentIndex(1);
            }
        }
    }

    SwipeView
    {
        clip: true
        interactive: false
        orientation: Qt.Vertical
        anchors.left: tabBar.right
        anchors.right: parent.right
        anchors.top: windowTitle.bottom
        anchors.bottom: tabBar.bottom
        currentIndex: tabBar.currentIndex

        Page
        {
            DiscoverPage
            {
                anchors.fill: parent
            }
        }

        Page
        {
            clip: true
            TransferPage
            {
                anchors.fill: parent
            }
        }
    }
}
