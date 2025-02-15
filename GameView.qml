import QtQuick 2.15

Rectangle {
    id: gameView
    width: parent.width*0.6
    height: parent.height
    color: "#111"
    border.color: "#333"

    GridView{
        id: grid
        anchors.fill: parent
        cellWidth: gameView.width/10
        cellHeight: gameView.height/20
        flow: GridView.FlowLeftToRight
        interactive: false
        model: gridmodel
        delegate: Rectangle{
            id: block
            width: gameView.width/10
            height: gameView.height/20
            color: model.colorCell
            border.color: "darkgrey"
            border.width: 1
            radius: width * 0.2

            // Animate only when row is clearing
            SequentialAnimation {
                id: clearAnimation
                running: model.isClear
                onRunningChanged: if (!running) block.scale = 1  // Reset scale after animation

                NumberAnimation { target: block; property: "scale"; to: 0.5; duration: 250; easing.type: Easing.OutQuad }
            }

        }
    }
}
