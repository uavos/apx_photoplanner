import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12


MapItemGroup {
    id: root
    property var map: ui.map

    property var pointsList: []

    MapItemView {
        id: borderPointsView
        model: apx.tools.photoplanner.borderPoints
        z: 1
        delegate: MapQuickItem {
            z: 1
            id: borderPointDelegate
            coordinate: coord
            sourceItem: Image {
                source: "qrc:/icons/place.svg"
                sourceSize.width: 40
                sourceSize.height: 40
            }
            anchorPoint.x: 20
            anchorPoint.y: 40

            MouseArea {
                anchors.fill: parent
                drag.target: borderPointDelegate
                drag.axis: Drag.XAndYAxis
                z: parent.z
                cursorShape: drag.active ? Qt.ClosedHandCursor : Qt.OpenHandCursor
                onReleased: {
                    coord = coordinate
                    var temp = root.pointsList
                    temp[index] = coordinate
                    root.pointsList = temp
                }
                onClicked: apx.tools.photoplanner.createRemoveDialog(borderPointDelegate.coordinate)
            }
            Component.onCompleted: {
                var temp = root.pointsList
                temp.splice(index, 0, coord)
                root.pointsList = temp
            }
            Component.onDestruction: {
                var index = root.pointsList.indexOf(coordinate)
                var temp = root.pointsList
                temp.splice(index, 1)
                root.pointsList = temp
            }
        }
    }

    MapItemView {
        id: photoPrintsView
        model: apx.tools.photoplanner.photoPrints
        z: 0
        delegate: MapPolygon {
            z: 0
            path: [p1, p2, p3, p4]
            color: "#2021be2b"
            border.width: 2
            border.color: photoPrintArea.containsMouse ? "yellow" : "#8021be2b"

            MouseArea {
                id: photoPrintArea
                anchors.fill: parent
                hoverEnabled: true
                z: parent.z
            }
        }
    }

    MapPolygon {
        id: polygon
        visible: apx.tools.photoplanner.missionType === "area"
        path: root.pointsList
        color: "#8021be2b"
        opacity: 0.5
        border.width: 3
        border.color: "darkgreen"
    }

    MapPolyline {
        id: polyline
        visible: apx.tools.photoplanner.missionType === "linear"
        path: root.pointsList
        line.color: "darkgreen"
        line.width: 3
        opacity: 0.5
    }

    Component.onCompleted: {
        map.addMapItemView(borderPointsView)
        map.addMapItemView(photoPrintsView)
        map.addMapItem(polygon)
    }
}
