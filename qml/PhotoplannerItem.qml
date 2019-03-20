import QtQuick 2.5
import QtLocation 5.12
import QtPositioning 5.6
import QtGraphicalEffects 1.0

//import Apx.Map 1.0
//import Apx.Common 1.0

MapCircle {
    id: control
    center: QtPositioning.coordinate(modelData.lat, modelData.lon)
    color: "red"
    radius: 10
    MouseArea {
        anchors.fill: parent
        drag.target: control
        drag.axis: Drag.XAndYAxis
    }

//    implicitZ: -100

//    implicitCoordinate: QtPositioning.coordinate(modelData.lat,modelData.lon)
//    title: modelData.title


//    textColor: "white"
//    color: Style.cGreen //"white"
//    hoverScaleFactor: 1
//    opacity: ui.effects?((hover||selected)?1:0.7):1

//    onMenuRequested: {
//        apx.tools.sites.edit.requestMenu({"closeOnActionTrigger":true, "pos":Qt.point(0,ui.window.height)})
//    }

    //dragging support
//    onMovingFinished: {
//        if(selected){
//            var d=modelData
//            d.lat=coordinate.latitude
//            d.lon=coordinate.longitude
//            apx.tools.sites.lookup.dbModel.set(index,d)
//        }
//    }
//    draggable: selected

//    onSelectedChanged: {
//        if(selected){
//            apx.tools.photoplanner.createEditor(modelData)
//        }else{
//            apx.tools.photoplanner.destroyEditor(modelData)
//        }
//    }
}
