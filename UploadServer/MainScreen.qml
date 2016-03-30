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
        width: 320
        height: 30
        spacing: 40

        Button {
            id: __startButton
            width: 80
            height: 30
            text: qsTr("start")


            onClicked: {
                App.start();
            }
        }

        Button {
            id: __stopButton
            width: 80
            height: 30
            text: qsTr("stop")
            onClicked: {
            }
        }
        Button {
            id: __saveFileButton
            width: 80
            height: 30
            text: qsTr("Select file")
            onClicked: {
                __fileDialog.savefile();
            }
        }
    }

    FileDialog {
        id: __fileDialog

        function savefile() {
            title = qsTr("Save a file")
            visible = true;
        }

        selectExisting: false
        folder: shortcuts.home
        nameFilters: ["All files (*.*)"]
        onAccepted: {
            App.destPath = fileUrl;
        }
    }
}

