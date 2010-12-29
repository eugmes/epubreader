import Qt 4.7

Rectangle {
    id: libraryView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.window

    ListView {
        model: documentListModel
        delegate: Row {
            Text {text: display; color: activePalette.windowText}
        }

        anchors.fill: parent
    }
}
