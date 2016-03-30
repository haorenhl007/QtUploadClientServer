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
            bottom: __buttons.top
            horizontalCenter: parent.horizontalCenter
            margins: 30
        }
        width: 300
        color: "black"
        border { width: 2; color: "grey" }

        Flickable {
            anchors { fill: parent; margins: 5 }
            contentHeight: __messages.contentHeight
            clip: true
            onContentHeightChanged: {
                if (contentHeight > height)
                    contentY = contentHeight - height;
            }

            Text {
                id: __messages
                width: parent.width
                color: "lightgrey"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
            }
        }
    }

    Row {
        id: __buttons
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 20
        }
        width: 300
        height: 30
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

