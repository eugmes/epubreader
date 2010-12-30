import Qt 4.7

Item {
    property string file: null
    property string bookTitle: null

    signal clicked

    id: bookItem

    width: parent.width
    height: 102

    BookThumbnail {
        id: thumbnail

        file: bookItem.file
        placeholderIcon: "/icons/epubreader.svg"

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 4
        }
    }

    Text {
        id: titleText

        text: bookTitle;
        color: activePalette.windowText

        anchors {
            left: thumbnail.right
            leftMargin: 4
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: bookItem.clicked()
    }
}
