import QtQuick 2.5
import QtLocation 5.9
import QtPositioning 5.6
import QtQml 2.12


MapItemGroup {
    id: photoplannerItems
    property var map: ui.map

    z: -200

    MapItemView {
        id: points
        model: apx.tools.photoplanner.points
        //        model: [{"lat": 37.406367, "lon": -122.046297}]
        delegate:
            MapCircle {
                id: control
                center: QtPositioning.coordinate(lat, lon)
                color: "red"
                radius: 10
//                onCenterChanged: console.log(lat, lon)
                MouseArea {
                    anchors.fill: parent
                    drag.target: control
                    drag.axis: Drag.XAndYAxis
                }
        }
    }

    Component.onCompleted: {
        //        apx.tools.sites.lookup.area=Qt.binding(function(){return apx.tools.map.area})
        map.addMapItemView(points)
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
