import QtQuick 2.12

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

        onPaint:
        {
            var ctx = getContext("2d");
            ctx.fillStyle = "#10FFFFFF";
            ctx.fillRect(0, 0, width, height);

            for(var i = 0; i < width; i += 20)
                drawLine(ctx, '#7266fc', 0.5,i + 0.5, 0, i + 0.5, height);
            for(var j = 0; j < height; j += 20)
                drawLine(ctx, '#7266fc', 0.5, 0, j + 0.5, width, j + 0.5);

            if (drawable)
            {
                var halfW = width / 2;
                var halfH = height / 2;

                ctx.lineWidth = 2;
                ctx.strokeStyle = '#72d6fc';
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
            discoverCon.discover();
        }
    }
}
