import QtQuick 2.5

import Apx.Common 1.0
//import Apx.Map 1.0

AppPlugin {
    id: photoplannerPlugin
    sourceComponent: Component { PhotoplannerMapItems { } }

    uiComponent: "map"
    onConfigure: parent = ui.map
    onLoaded: ui.map.addMapItem(item)
}
