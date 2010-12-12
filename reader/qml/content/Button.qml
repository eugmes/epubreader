import Qt 4.7

Item {
    id: button
    property alias image: icon.source
    property variant action

    signal clicked

    width: 120
    height: parent.height

    Image {
        id: icon
        anchors.centerIn: parent
        opacity:  if (action != undefined) {action.enabled ? 1.0 : 0.4} else 1
    }

    MouseArea {
        anchors {fill: parent}
        onClicked: {
            if (action != undefined && action.enabled)
                action.trigger()
            button.clicked()
        }
    }
}
