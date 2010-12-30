import Qt 4.7
import "content"

Rectangle {
    id: libraryView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.window

    ListView {
        model: documentListModel
        delegate: Item {
            width: libraryView.width
            height: 102

            BookThumbnail {
                id: thumbnail

                file: fileName
                placeholderIcon: "/icons/epubreader.svg"

                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 4
                }
            }

            Text {
                id: titleText

                text: display;
                color: activePalette.windowText

                anchors {
                    left: thumbnail.right
                    leftMargin: 4
                }
            }
        }

        anchors.fill: parent
        anchors.margins: 4
    }
}
