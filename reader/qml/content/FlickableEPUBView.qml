import Qt 4.7
import EPUBReader 1.0

Flickable {
    id: flickable

    property alias fileName: epubView.fileName
    property alias prevPage: epubView.prevPage
    property alias nextPage: epubView.nextPage
    property alias textSizeMultiplier: epubView.textSizeMultiplier
    property alias defaultFont: epubView.defaultFont
    property alias backgroundIndex: epubView.backgroundIndex
    property alias tocDocument: epubView.tocDocument
    property alias url: epubView.url

    function openTocDocumentRequest(path) {
        epubView.openTocDocumentRequest(path)
    }

    clip: true
    contentWidth: parent.width // Do not scroll in this direction
    contentHeight: Math.max(parent.height, epubView.height)
    pressDelay: 400 // Avoid selection

    /* TODO add key press handler */

    // FIXME Item is needed for Maemo
    Item {
        id: epubItem
        EPUBView {
            id: epubView

            preferredWidth: flickable.width
            preferredHeight: flickable.height

            onUrlChanged: {
                flickable.contentX = 0
                flickable.contentY = 0
            }
        }
    }
}
