import Qt 4.7

Rectangle {
    id: tocView

    SystemPalette {id: activePalette; colorGroup: SystemPalette.Active}

    color: activePalette.window

    XmlListModel {
        id: tocModel
        xml: tocWindow.document // FIXME assumes UTF8
        namespaceDeclarations: "declare default element namespace \"http://www.daisy.org/z3986/2005/ncx/\";"
        query: "/ncx/navMap/navPoint"

        XmlRole {name: "pointId"; query: "@id/string()"}
        XmlRole {name: "playOrder"; query: "@playOrder/number()"}
        XmlRole {name: "labelText"; query: "navLabel/text/string()"}
        XmlRole {name: "contentUrl"; query: "content/@src/string()"}
    }

    ListView {
        id: tocList

        model: tocModel
        delegate: Text {
            color: activePalette.windowText
            width: parent.width

            font {
                /* system font */
                family: "Nokia Sans"
                pointSize: 18
            }

            text: labelText
        }
        anchors.fill: parent
    }
}
