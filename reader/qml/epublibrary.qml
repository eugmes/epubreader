import Qt 4.7
import "content"

Rectangle {
    id: libraryView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.window

    ListView {
        model: documentListModel
        delegate: BookItem {
            file: fileName
            bookTitle: display

            onClicked: libraryBrowser.openFile(fileName)
        }

        anchors.fill: parent
        anchors.margins: 4
    }
}
