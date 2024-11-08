import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1

import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 450
    height: 300
//    visibility: "FullScreen"

    color: "black"


        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.fill:parent
        }

//        Rectangle {
//            color: Qt.rgba(1, 1, 1, 0.7)
//            border.width: 1
//            border.color: "white"
//            anchors.bottom: video.bottom
//            anchors.bottomMargin: 15
//            anchors.horizontalCenter: parent.horizontalCenter
//            width : parent.width - 30
//            height: parent.height - 30
//            radius: 8

//            MouseArea {
//                id: mousearea
//                anchors.fill: parent
//                hoverEnabled: true
//                onEntered: {
//                    parent.opacity = 1.0
//                    hidetimer.start()
//                }
//            }

//            Timer {
//                id: hidetimer
//                interval: 5000
//                onTriggered: {
//                    parent.opacity = 0.0
//                    stop()
//                }
//            }


}
