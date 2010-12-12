import Qt 4.7

Rectangle {
    ListView {
        model: documentListModel
        delegate: Row {
            Text {text: display}
        }

        anchors.fill: parent
    }
}
