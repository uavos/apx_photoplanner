import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12


MapItemGroup {
    id: root
    property var map: ui.map

    property var pointsList: []

//    z: -200

    MapItemView {
        id: borderPointsView
        model: apx.tools.photoplanner.borderPoints
        z: 1
        delegate: MapQuickItem {
            z: 1
            id: borderPointDelegate
            coordinate: coord
//            onCoordinateChanged: {
//                coord = coordinate
//                var temp = root.pointsList
//                temp[index] = coordinate
//                root.pointsList = temp
//            }
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
            }
            Component.onCompleted: {
                var temp = root.pointsList
                temp.splice(index, 0, coord)
                root.pointsList = temp
            }
            Component.onDestruction: {
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
            color: "green"
            opacity: 0.5
            border.width: 2
            border.color: photoPrintArea.containsMouse ? "yellow" : Qt.darker("green")

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
        path: root.pointsList
        color: "transparent"
        opacity: 0.3
        border.width: 3
        border.color: Qt.darker("blue")
    }

    Component.onCompleted: {
        //        apx.tools.sites.lookup.area=Qt.binding(function(){return apx.tools.map.area})
        map.addMapItemView(borderPointsView)
        map.addMapItemView(photoPrintsView)
        map.addMapItem(polygon)
    }

    //triggered site in lookup - focus on map
//    Connections {
        //        target: apx.tools.sites.lookup
        //        onItemTriggered: map.showCoordinate(QtPositioning.coordinate(modelData.lat,modelData.lon))
//    }


    //CURRENT SITE LABEL
    //    Text {
    //        //z: -1000
    //        parent: map //ui.main
    //        anchors.horizontalCenter: parent.horizontalCenter
    //        anchors.bottom: parent.bottom
    //        anchors.bottomMargin: ui.main.containerBottom.height ? ui.main.containerBottom.height + 20 : 0
    //        visible: text
    //        font.pixelSize: 24
    //        font.bold: true
    //        color: "#fff"
    //        text: "trololo"/*apx.vehicles.current.mission.site*/
    //    }

}
