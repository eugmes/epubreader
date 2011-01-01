import Qt 4.7

Item {
    property string file: null
    property string bookTitle: null
    property string bookAuthor: null

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

    Column {
        Text {
            id: titleText

            text: bookTitle;
            color: activePalette.windowText

            font {
                /* system font */
                family: "Nokia Sans"
                pointSize: 18
            }
        }

        Text {
            id: additionalText

            text: bookAuthor
            color: disabledPalette.windowText

            font {
                /* small system font */
                family: "Nokia Sans"
                pointSize: 13
            }
        }

        anchors {
            left: thumbnail.right
            leftMargin: 8
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: bookItem.clicked()
    }

    anchors {
        topMargin: 2
        bottomMargin: 2
    }
}
