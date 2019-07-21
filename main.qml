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

    property real t: 0

    SequentialAnimation on t {
        NumberAnimation { to: 1; duration: 2500; }
        NumberAnimation { to: 0; duration: 2500; }
        loops: Animation.Infinite
        running: true
    }

    onTChanged: {
        _renderer.t = t;
    }

    Rectangle{
        width: 100
        height: 100
        color: "blue"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                console.log( "clicked");
            }
        }
    }
}
