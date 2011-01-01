import Qt 4.7

Item {
    id: item
    signal clicked(string path)

    width: parent.width
    height: label.height

    Text {
        id: label

        color: activePalette.windowText
        text: labelText

        font {
            /* system font */
            family: "Nokia Sans"
            pointSize: 18
        }
    }

    MouseArea {
        onClicked: item.clicked(contentUrl)
        anchors.fill: parent
    }
}
