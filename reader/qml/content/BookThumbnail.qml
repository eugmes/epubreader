import Qt 4.7
import EPUBReader 1.0

Item {
    property alias file: thumbnail.file
    property alias placeholderIcon: placeholder.source

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

        anchors.fill: parent
    }
}
