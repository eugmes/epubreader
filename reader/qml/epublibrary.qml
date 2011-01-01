import Qt 4.7
import "content"

Rectangle {
    id: libraryView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}
    SystemPalette {id: disabledPalette; colorGroup: SystemPalette.Disabled}

    color: activePalette.window

    ListView {
        id: list

        model: documentListModel
        delegate: BookItem {
            file: fileName
            bookTitle: display
            bookAuthor: author

            onClicked: libraryBrowser.openFile(fileName)
        }

        anchors.fill: parent
    }
}
