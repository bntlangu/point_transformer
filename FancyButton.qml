import QtQuick 2.0

Item {

    id: btnRoot
    property string text
    property color bgColor: "black"
    property color bgColorSelected: "blue"
    property color textColor: "white"
    property color textColorSelected: "silver"
    property alias enabled: mouseArea.enabled
    property alias radius: bgr.radius

    signal clicked

    Rectangle {
        id: bgr
        anchors.fill: parent
        color: mouseArea.pressed ? bgColorSelected : bgColor
        radius: height / 15

        Text {
            id: text
            anchors.centerIn: parent
            text: btnRoot.text
            font.pixelSize: 0.4 * parent.height
            color: mouseArea.pressed ? textColorSelected : textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                btnRoot.clicked()
            }
        }
    }
}








/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
