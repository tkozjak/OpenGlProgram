import QtQuick 2.13
import QtQuick.Window 2.13

Window {
    objectName: "OpenGl Window"

    visible: true
//    visibility: Window.FullScreen
    width: 640
    height: 480
    title: qsTr("OpengGl Test Window")

//    color: "transparent"

    property real time: 0

    SequentialAnimation on time {
        NumberAnimation { to: 1; duration: 2500; }
        NumberAnimation { to: 0; duration: 2500; }
        loops: Animation.Infinite
        running: true
    }

    onTimeChanged: {
        _renderer.time = time;
    }

    Rectangle{
        x: 10
        y: 10
        width: 50
        height: 50
        color: "blue"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                _renderer.gswitch = !(_renderer.gswitch );
            }
        }
    }
}
