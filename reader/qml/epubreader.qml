import Qt 4.7
import "content"

Rectangle {
    id: mainView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.base // TODO maybe window?

    FlickableEPUBView {
        id: epubView

        fileName: mainWindow.fileName
        textSizeMultiplier: settings.textSizeMultiplier
        defaultFont: settings.fontFamily
        backgroundIndex: settings.colorIndex

        anchors {top: parent.top; left: parent.left; right: parent.right; bottom: toolBar.top}
    }

//    ScrollBar {
//        width: 8
//        anchors {right: parent.right; top: parent.top; bottom: toolBar.top}
//    }

    Rectangle {
        id: toolBar
        height: 70
        color: activePalette.window

        anchors {bottom: parent.bottom; left: parent.left; right: parent.right}

        Row {
            anchors.fill: parent

            Button {
                id: prevButton
                image: "/pics/qgn_back_fsm.png"
                action: epubView.prevPage
            }

            Button {
                id: tocButton
                image: "/pics/qgn_back_fsm.png" // TODO

                onClicked: {
                    mainWindow.showToc(epubView.tocUrl)
                }
            }

            Button {
                id: nextButton
                image: "/pics/qgn_forward_fsm.png"
                action: epubView.nextPage
            }
        }
        Button {
            id: fullSizeButton
            image: "/pics/general_fullsize.png"
            onClicked: {
                mainWindow.fullSize = true
            }

            anchors.right: parent.right
        }
    }

    Item {
        id: fullSizeToolBar
        visible: false
        height: 70
        width: 120

        Button {
            id: normalSizeButton
            image: "/pics/general_fullsize_b.png"
            opacity: 0.4

            onClicked: {
                mainWindow.fullSize = false
            }
        }

        anchors {bottom: parent.bottom; horizontalCenter: parent.horizontalCenter}
    }

    states: State {
        name: "fullSize";
        when: mainWindow.fullSize
        PropertyChanges {target: fullSizeToolBar; visible: true}
        PropertyChanges {target: toolBar; visible: false}
        PropertyChanges {target: epubView; anchors.bottom: mainView.bottom}
    }

    function fileNameChange() {

    }

    Component.onCompleted: mainWindow.openTocDocumentRequest.connect(epubView.openTocDocumentRequest)

    Component.onDestruction: {
        console.log("Destruction", epubView.fileName, epubView.url)
        settings.saveLastURL(epubView.fileName, epubView.url)
    }
}
