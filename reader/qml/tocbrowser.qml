import Qt 4.7
import "content"

Rectangle {
    id: tocView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.window

    ListView {
        id: tocList

        model: TOCModel {source: tocWindow.tocUrl}
        delegate: TOCItem {
            onClicked: tocWindow.openTocDocument(path)
        }

        spacing: 4

        anchors.fill: parent
    }
}
