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

    Rectangle {
        id: __progressBar
        anchors { centerIn: parent; verticalCenterOffset: 50 }
        width: 300
        height: 40
        color: "lightgrey"
        border { width: 2; color: "grey" }

//        Rectangle {
//            anchors {
//                top: parent.top
//                left: parent.left
//            }
//            width: parent.width * App.currentWritten / App.totalSize
//            height: parent.height
//            color: "blue"
//        }

        Label {
            anchors.centerIn: parent
            color: "white"
            font.pixelSize: 16
            text: "Uploaded: " + App.progressString
        }
    }

    Row {
        id: __buttons
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: __progressBar.bottom
            topMargin: 20
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
            text: qsTr("Select file")
            onClicked: {
                __fileDialog.selectFile();
            }
        }
    }

    Row {
        id: __connectionSettings
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: __buttons.bottom
            margins: 20

        }
        width: parent.width - 20
        height: 30
        spacing: 10

        Text {
            width: 60
            height: parent.height
            color: "black"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            text: qsTr("Address:")
        }

        Rectangle {
            width: 100
            height: parent.height
            color: "lightgrey"
            border { width: 2; color: "grey" }
            clip: true

            TextInput {
                id: __host
                anchors.fill: parent
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                onTextChanged: {
                    App.host = text;
                }
                Component.onCompleted: {
                    __host.text = App.host;
                }
            }
        }

        Text {
            width: 30
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
                id: __port
                anchors.fill: parent
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                onTextChanged: {
                    App.port = text;
                }
                Component.onCompleted: {
                    __port.text = App.port;
                }
            }
        }

    }






    FileDialog {
        id: __fileDialog

        function selectFile() {
            title = qsTr("Select a file")
            visible = true;
        }

        selectExisting: true
        folder: shortcuts.home
        nameFilters: ["All files (*.*)"]
        onAccepted: {
            App.filePath = fileUrl;
        }

        onRejected: {
        }
    }
}

