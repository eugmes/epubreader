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
    property alias tocUrl: epubView.tocUrl

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

        NavigationButton {
            id: topPrevButton
            image: "/pics/qgn_back_fsm.png"
            action: epubView.prevPage

            x: 0
            anchors {top: epubView.top}
        }

        NavigationButton {
            id: topNextButton
            image: "/pics/qgn_forward_fsm.png"
            action: epubView.nextPage

            x: flickable.width - width
            anchors {top: epubView.top}
        }

        NavigationButton {
            id: bottomPrevButton
            image: "/pics/qgn_back_fsm.png"
            action: epubView.prevPage

            x: 0
            anchors {bottom: epubView.bottom}
        }

        NavigationButton {
            id: bottomNextButton
            image: "/pics/qgn_forward_fsm.png"
            action: epubView.nextPage

            x: flickable.width - width
            anchors {bottom: epubView.bottom}
        }
    }
}
