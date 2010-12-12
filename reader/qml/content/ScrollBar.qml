import Qt 4.7

Item {
    id: scrollBar

    property real position
    property real pageSize

    Rectangle {
        id: background
        anchors.fill: parent
        radius: width/2 - 1
        color: "black"
        opacity: 0.3
    }

    Rectangle {
        x: 1
        y: scrollBar.position * (scrollBar.height - 2) + 1
        width: parent.width - 2
        height: scrollBar.pageSize * (scrollBar.height - 2)
        radius: width/2 - 1
        color: "black"
        opacity: 0.7
    }
}
