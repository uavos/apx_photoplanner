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
        Connections {
            target: apx.tools.photoplanner.borderPoints
            onPointsChanged: pointsList = apx.tools.photoplanner.borderPoints.getAllPointsAsVariants()
        }

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
                drag.onActiveChanged: {
                    if(!drag.active) {
                        coord = coordinate
                    }
                }
                onClicked: {
                    apx.tools.photoplanner.createEditor(id, borderPointDelegate.coordinate);
                    apx.tools.photoplanner.point_edit.requestMenu({"closeOnActionTrigger":true});
                }
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
        path: root.pointsList.length >= 3 ? root.pointsList : []
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

    Rectangle {
        id: photoplannerInfo
        color: "#376479"
        parent: ui.map.parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: stats.width
        height: stats.height
        opacity: 0.8
        radius: 6
        Text {
            property var totalDistance: apx.tools.photoplanner.totalDistance
            property var area: apx.tools.photoplanner.borderPoints.area
            id: stats
            anchors.centerIn: parent
            textFormat: Text.RichText
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(apx.tools.photoplanner.missionType == "linear")
                    return String("Photoplan<br>Mission length: %1m").arg(totalDistance);
                else if(apx.tools.photoplanner.missionType == "area")
                {
                    return String("Photoplan<br>Mission length: %1m<br>Area: %2m<sup>2</sup>")
                    .arg(totalDistance)
                    .arg(area);
                }
            }
            color: "white"
        }
    }

    Component.onCompleted: {
        map.addMapItemView(borderPointsView);
        map.addMapItemView(photoPrintsView);
        map.addMapItem(polygon);
        map.addMapItem(polyline);
    }
}
