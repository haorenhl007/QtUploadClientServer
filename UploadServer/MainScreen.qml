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
            bottom: __connectionSettings.top
            bottomMargin: 20
        }
        width: 320
        height: 30
        spacing: 40

        Button {
            id: __startButton
            width: 80
            height: 30
            text: qsTr("Listen")


            onClicked: {
                App.start();
            }
        }

        Button {
            id: __stopButton
            width: 80
            height: 30
            text: qsTr("Stop")
            onClicked: {
                App.stop();
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

    Row {
        id: __connectionSettings
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            margins: 20
        }
        width: parent.width - 40
        height: 30
        spacing: 10

        Text {
            width: 40
            height: parent.height
            color: "black"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            text: qsTr("port:")
        }

        Rectangle {
            width: 100
            height: parent.height
            color: "lightgrey"
            border { width: 2; color: "grey" }
            clip: true

            TextInput {
                id: __portInput
                anchors.fill: parent
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                onTextChanged: {
                    App.port = text;
                }
                Component.onCompleted: {
                    __portInput.text = App.port;
                }
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
        nameFilters: ["All files (*)"]
        onAccepted: {
            App.destPath = fileUrl;
        }
    }
}

