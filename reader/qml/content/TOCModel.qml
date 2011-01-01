import Qt 4.7

XmlListModel {
    namespaceDeclarations: "declare default element namespace \"http://www.daisy.org/z3986/2005/ncx/\";"
    query: "/ncx/navMap//navPoint" // FIXME

    XmlRole {name: "pointId"; query: "@id/string()"}
    XmlRole {name: "playOrder"; query: "@playOrder/number()"}
    XmlRole {name: "labelText"; query: "navLabel/text/string()"}
    XmlRole {name: "contentUrl"; query: "content/@src/string()"}
}
