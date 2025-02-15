import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: tetrisWindow
    visible: true
    width: 500
    height: 600
    title: "Tetris"
    color : "#3498db"

    Rectangle{
        id: background
        color: "#1A1A1A" // Dark theme
        anchors.fill: parent

        Row {
            id: tetrisRow
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

            GameView {
            }

            Controls {
            }
        }
        focus: true
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Left) tetrisGame.moveLeft();
            else if (event.key === Qt.Key_Right) tetrisGame.moveRight();
            else if (event.key === Qt.Key_Down) tetrisGame.moveDown();
            else if (event.key === Qt.Key_Up) tetrisGame.rotate();
            else if (event.key === Qt.Key_Space) tetrisGame.drop();
        }
    }
}
