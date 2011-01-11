import Qt 4.7
import EPUBReader 1.0
import "Icons.js" as Icons

Item {
    property alias file: thumbnail.file

    width: 96
    height: 96

    ThumbnailItem {
        id: thumbnail
        visible: loaded

        anchors.fill: parent
    }

    Image {
        id: placeholder
        visible: !thumbnail.loaded
        smooth: true

        source: "image://hildon-icon/" + Icons.unknownBookIcon

        sourceSize {
            width: 96
            height: 96
        }

        anchors.fill: parent
    }
}
