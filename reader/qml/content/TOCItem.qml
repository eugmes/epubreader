import Qt 4.7

Rectangle {
    id: item
    signal clicked(string path)

    height: 70
    /* TODO border color */

    color: activePalette.button
    radius: 3

    Text {
        id: label

        color: activePalette.buttonText
        text: display

        font {
            /* system font */
            family: "Nokia Sans"
            pointSize: 18
        }

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter

            leftMargin: 8
        }
    }

    MouseArea {
        onClicked: item.clicked(contentUrl)
        anchors.fill: parent
    }

    anchors {
        leftMargin: 2
        rightMargin: 2

        left: parent.left
        right: parent.right
    }
}
