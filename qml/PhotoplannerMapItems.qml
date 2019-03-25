import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12


MapItemGroup {
    id: root
    property var map: ui.map

    property var pointsList: []

    z: -200

    MapItemView {
        id: points
        model: apx.tools.photoplanner.points
        delegate:
            MapQuickItem {
            id: control
            coordinate: QtPositioning.coordinate(lat, lon)
            onCoordinateChanged: {
                lat = coordinate.latitude
                lon = coordinate.longitude
                var temp = root.pointsList
                temp[index] = coordinate
                root.pointsList = temp
            }
            sourceItem: Image {
                source: "qrc:/icons/place.svg"
                sourceSize.width: 40
                sourceSize.height: 40
            }
            anchorPoint.x: 20
            anchorPoint.y: 40

            MouseArea {
                anchors.fill: parent
                drag.target: control
                drag.axis: Drag.XAndYAxis
            }
            Component.onCompleted: {
                var temp = root.pointsList
                temp.splice(index, 0, QtPositioning.coordinate(lat, lon))
                root.pointsList = temp
            }
            Component.onDestruction: {
                var temp = root.pointsList
                temp.splice(index, 1)
                root.pointsList = temp
            }
        }
    }

    MapPolygon {
        id: polygon
        //        path: repeater.coords
        path: root.pointsList
        color: "blue"
        opacity: 0.3
        border.width: 3
        border.color: Qt.darker("blue")
    }

    Component.onCompleted: {
        //        apx.tools.sites.lookup.area=Qt.binding(function(){return apx.tools.map.area})
        map.addMapItemView(points)
        map.addMapItem(polygon)
    }

    //triggered site in lookup - focus on map
    Connections {
        //        target: apx.tools.sites.lookup
        //        onItemTriggered: map.showCoordinate(QtPositioning.coordinate(modelData.lat,modelData.lon))
    }


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
