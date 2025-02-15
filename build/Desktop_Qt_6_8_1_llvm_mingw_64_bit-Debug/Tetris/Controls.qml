import QtQuick 2.15
import QtQuick.Controls 2.3
import QtQuick.Controls.Basic

Rectangle {
    id: controls
    width: parent.width * 0.4
    height: parent.height
    color: "transparent"
    function changeTetromino(tetroType) {
        let shapes = {
            0: [  // I
                [1, 1, 1, 1],
            ],
            1: [  // O
                [1, 1],
                [1, 1]
            ],
            2: [  // T
                [0, 1, 0],
                [1, 1, 1]
            ],
            3: [  // S
                [0, 1, 1],
                [1, 1, 0]
            ],
            4: [  // Z
                [1, 1, 0],
                [0, 1, 1]
            ],
            5: [  // J
                [1, 0, 0],
                [1, 1, 1]
            ],
            6: [  // L
                [0, 0, 1],
                [1, 1, 1]
            ]
        };

        let colors = {
            0: "cyan",   // I
            1: "yellow", // O
            2: "magenta", // T
            3: "green",  // S
            4: "red",    // Z
            5: "blue",   // J
            6: "orange"  // L
        };

        let shape = shapes[tetroType] || [[]];
        let color = colors[tetroType] || "black";

        tetroModel.clear();

        let numRows = shape.length;
        let numCols = shape[0] ? shape[0].length : 0;

        nextTetro.numCols = numCols;
        nextTetro.numRows = numRows;

        for (let r = 0; r < numRows; r++) {
            for (let c = 0; c < numCols; c++) {
                tetroModel.append({
                    color: shape[r][c] ? color : "transparent"
                });
            }
        }
    }

    ListModel {
        id: tetroModel
    }

    Column {
        id: sidebarColumn
        spacing: 20
        anchors.fill: parent
        anchors.margins: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: "Next"
            font.pixelSize: 30
            font.family: "Arial"
            color: "white"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
        Item {
            width: controls.width
            height: controls.height*0.2
            anchors.horizontalCenter: parent.horizontalCenter
            // border.color: "red"
            GridView {
                id: nextTetro

                property int numRows: 0
                property int numCols: 0

                width: (controls.width/8)*numCols
                height: (controls.height*0.2/4)*numRows
                z:0
                cellWidth: controls.width/8
                cellHeight: controls.height*0.2/4
                model: tetroModel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                interactive: false
                flow: GridView.FlowLeftToRight
                delegate: Rectangle {
                    width: controls.width/8
                    height: controls.height*0.2/4
                    color: model.color
                    border.color: "darkgrey"
                    border.width: 1
                    radius: width * 0.2
                    visible :  model.color == "transparent" ? false : true
                }
            }
            Rectangle {
                id: background
                width: sidebarColumn.width * 0.8
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                z: -1
                color: "#222"
                border.color: "#333"
            }
        }
        Text {
            text: "Score"
            font.pixelSize: 30
            color: "white"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: score
            color: "#222"
            border.color: "#333"
            width: sidebarColumn.width * 0.8
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                anchors.centerIn: parent
                text: tetrisGame ? tetrisGame.score : "0"
                font.pixelSize: 24
                color: "yellow"
                font.bold: true
            }
        }
    }

    Column {
        id: buttonRow
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20

        Button {
            id: startbutton
            text: tetrisGame ? (tetrisGame.gameOver ? "Start" : "Restart") : "Start"
            focusPolicy: Qt.NoFocus
            height: 40
            width: controls.width * 0.5
            background: Rectangle {
                implicitWidth: controls.width * 0.5
                implicitHeight: 40
                color: startbutton.down ? "#444" : "#222"
                border.color: "#444"
                border.width: 1
                radius: width*0.8
                gradient: Gradient {
                    GradientStop { position: 0.0; color: startbutton.down ? "#222" : "#333" }
                    GradientStop { position: 1.0; color: startbutton.down ? "#000" : "#111" }
                }
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.centerIn: parent
            }
            onClicked: {
                console.log("Game restarted");
                tetrisGame.reset();
            }
        }

        Button {
            id: pauseButton
            text: tetrisGame ? (tetrisGame.pauseGame ? "Resume" : "Pause") : "Pause"
            focusPolicy: Qt.NoFocus
            height: 40
            width: controls.width * 0.5
            background: Rectangle {
                implicitWidth: controls.width * 0.5
                implicitHeight: 40
                color: startbutton.down ? "#444" : "#222"
                border.color: "#444"
                border.width: 1
                radius: width*0.8
                gradient: Gradient {
                    GradientStop { position: 0.0; color: pauseButton.down ? "#222" : "#333" }
                    GradientStop { position: 1.0; color: pauseButton.down ? "#000" : "#111" }
                }
            }
            contentItem: Text {
                text: parent.text
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.centerIn: parent
            }
            onClicked: {
                if (tetrisGame) {
                    if (!tetrisGame.gameOver){
                        if (tetrisGame.pauseGame) {
                            tetrisGame.resume();
                        } else {
                            tetrisGame.pause();
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: tetrisGame
        function onNewTetrominoChanged(type){
            changeTetromino(type)
        }
    }
}
