import Qt 4.7

Rectangle {
    id: scrollBar

    property Flickable area: null

    height: area.height * area.visibleArea.heightRatio
    y: area.y + area.height * area.visibleArea.yPosition

    width: 6

    color: "gray" // FIXME
    border {
        color: "black"
        width: 1
    }

    opacity: 0

    anchors {
        right: area.right
        rightMargin: 3
    }

    states: State {
        name: "visible"
        PropertyChanges {target: scrollBar; opacity: 0.4}
    }

    transitions: Transition {
        PropertyAnimation {properties: "opacity"; easing.type: Easing.InOutQuad}
    }
}
