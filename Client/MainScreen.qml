import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.3
import com.qtuploadClientServer 1.0


Item {
    Rectangle {
        id: __progressBar
        anchors.centerIn: parent
        width: 300
        height: 40
        color: "lightgrey"

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
            text: "Uploaded: " + App.progressString
        }
    }

    Row {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: __progressBar.bottom
            topMargin: 40
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
            text: qsTr("Select file")
            onClicked: {
            }
        }
    }
}

