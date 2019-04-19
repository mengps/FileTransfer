import QtQuick 2.12
import QtQuick.Controls 2.12

Item
{
    id: root
    clip: true

    Canvas
    {
        id: canvas
        anchors.fill: parent
        antialiasing: true
        property bool drawable: false;
        property bool first: true;
        property real r: Math.sqrt(Math.pow(width / 2, 2) + Math.pow(height / 2, 2));
        property real r1: 0;
        property real r2: r / 2;

        function clear()
        {
            first = true;
            r1 = 0;
            r2 = r / 2;
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
        }

        function drawLine(ctx, color, width, startX, startY, endX, endY)
        {
            ctx.strokeStyle = color;
            ctx.lineWidth = width;
            ctx.beginPath();
            ctx.moveTo(startX, startY);
            ctx.lineTo(endX, endY);
            ctx.closePath();
            ctx.stroke();
        }

        Timer
        {
            id: timer
            interval: 16
            running: false
            repeat: true
            onTriggered: canvas.requestPaint();
        }

        /*onPaint:
        {
            var ctx = getContext("2d");
            ctx.fillStyle = "#10FFFFFF";
            ctx.fillRect(0, 0, width, height);

            for(var i = 0; i < width; i += 20)
                drawLine(ctx, "#7266fc", 0.5,i + 0.5, 0, i + 0.5, height);
            for(var j = 0; j < height; j += 20)
                drawLine(ctx, "#7266fc", 0.5, 0, j + 0.5, width, j + 0.5);

            if (drawable)
            {
                var halfW = width / 2;
                var halfH = height / 2;

                ctx.lineWidth = 2;
                ctx.strokeStyle = "#72d6fc";
                for(var k = 0; k < 5; k += 0.5)
                {
                    ctx.beginPath();
                    ctx.arc(halfW, halfH, r1 + k, 0, Math.PI * 2);
                    ctx.closePath();
                    ctx.stroke();

                    ctx.beginPath();
                    if(!first) ctx.arc(halfW, halfH, r2 + k, 0, Math.PI * 2);
                    ctx.closePath();
                    ctx.stroke();
                }
                if(r1 > r) r1 = 0;
                if(r2 > r)
                {
                    r2 = 0;
                    if(first) first = false;
                }
                r1 += 3;
                r2 += 3;
            }
        }*/
    }

    GlowRectangle
    {
        id: nameLabel
        radius: 5
        height: 50
        color: "#D7C2F9"
        glowColor: color
        anchors.left: parent.left
        anchors.leftMargin: 70
        anchors.right: parent.right
        anchors.rightMargin: 70
        anchors.top: parent.top
        anchors.topMargin: -radius

        Row
        {
            clip: true
            width:  180
            height: 28
            anchors.centerIn: parent

            Text
            {
                id: nameText
                width:  50
                height: 28
                verticalAlignment: Text.AlignVCenter
                text: qsTr("昵称：")
            }

            FlatInput
            {
                id: nameInput
                width: 120
                height: 28
                selectByMouse: true
                text: qsTr("未命名")
                placeholderText: qsTr("给自己取个名字吧~")
                onTextChanged: discoverCon.name = text;
            }
        }
    }

    MyButton
    {
        text: "扫描"
        widthMargin: 12
        heightMargin: 8
        anchors.centerIn: parent
        onClicked:
        {
            canvas.clear();
            canvas.drawable = true;
            timer.restart();

            apBackground.visible = true;
            accessPoints.clear();
            discoverCon.discover();
        }
    }

    Connections
    {
        target: discoverCon
        onNewAccessPoint:
        {
            accessPoints.append({"name": name});
        }
    }

    Rectangle
    {
        id: apBackground
        clip: true
        radius: 5
        color: "#00FFFFFF"
        width: 150
        height: 300
        anchors.top: nameLabel.bottom
        anchors.topMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 50

        Text
        {
            id: apLabel
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("扫描到：")
        }

        ListView
        {
            id: listView
            clip: true
            anchors.top: apLabel.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            spacing: 4
            ScrollBar.vertical: ScrollBar
            {
                policy: ScrollBar.AsNeeded
            }
            displaced: Transition
            {
                NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
            }
            model: ListModel { id: accessPoints }
            delegate: Component
            {
                Rectangle
                {
                    width: listView.width - 20
                    height: 32
                    radius: 2
                    border.color: "#777"
                    color: hovered ? "#559EF2FA" : "#55556677"
                    property bool hovered: false

                    MouseArea
                    {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: parent.hovered = true;
                        onExited: parent.hovered = false;
                        onClicked:
                        {
                            fileTransfer.setAccessPoint(name);
                            accessPoints.remove(index);
                        }
                    }

                    Text
                    {
                        anchors.centerIn: parent
                        text: qsTr(name)
                    }
                }
            }
        }
    }
}
