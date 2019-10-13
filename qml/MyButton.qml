import QtQuick 2.12

GlowRectangle {
    id: root
    width: text.width + widthMargin * 2
    height: text.height + heightMargin * 2
    radius: 6
    glowRadius: 6
    color: hovered ? hoverColor : Qt.lighter(hoverColor, 1.18);

    property alias text: text.text;
    property alias fontSize: text.font.pointSize;
    property bool hovered: false;
    property int widthMargin: 10;
    property int heightMargin: 4;
    property color textColor: text.color;
    property color hoverColor: "#A0A0A0";

    signal clicked();
    signal pressed();
    signal released();
    signal entered();
    signal exited();

    Text {
        id: text
        font.family: "微软雅黑"
        color: "#333"
        x: widthMargin
        y: heightMargin
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            root.pressed();
            text.x += 1;
            text.y += 1;
        }
        onReleased: {
            root.released();
            root.clicked();
            text.x -= 1;
            text.y -= 1;
        }
        onEntered: root.hovered = true;
        onExited: root.hovered = false;
    }
}
