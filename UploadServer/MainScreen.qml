import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.3
import com.qtuploadClientServer 1.0


Item {
    id: screen

    Connections {
        target: App
        onNewMessage: {
            __messages.text += arg + "\n";
        }
    }

    Rectangle {
        id: __messageWindow
        anchors {
            top: parent.top
            bottom: __progressBar.top
            horizontalCenter: parent.horizontalCenter
            margins: 20
        }
        width: 300
        color: "black"
        border { width: 2; color: "grey" }

        Flickable {
            anchors { fill: parent; margins: 5 }
            contentHeight: __messages.contentHeight
            clip: true
            Text {
                id: __messages
                width: parent.width
                color: "green"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
            }
        }
    }


    Rectangle {
        id: __progressBar
        anchors { centerIn: parent; verticalCenterOffset: 50 }
        width: 300
        height: 40
        color: "lightgrey"
        border { width: 2; color: "grey" }

        Rectangle {
            anchors {
                top: parent.top
                left: parent.left
            }
            width: parent.width * App.currentWritten / App.totalSize
            height: parent.height
            color: "blue"
        }

        Label {
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 16
            text: "Received: " + App.progressString
        }
    }

    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: __progressBar.bottom
            topMargin: 20
        }
        width: 300
        spacing: 40

        Button {
            id: __startButton
            width: 130
            height: 30
            enabled: !App.buttonsAreLocked
            text: qsTr("start")


            onClicked: {
                App.start();
            }
        }

        Button {
            id: __selectFileButton
            width: 130
            height: 30
            enabled: !App.buttonsAreLocked
            text: qsTr("stop")
            onClicked: {
            }
        }
    }
}

