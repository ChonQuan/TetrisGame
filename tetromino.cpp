#include "tetromino.h"

void Tetromino::rotate() {
    currentRotation = (currentRotation + 1) % rotations.size();
}

void Tetromino::unRotate()
{
    currentRotation = (currentRotation - 1) % rotations.size();
    if (currentRotation == -1) currentRotation = rotations.size() - 1;
}

void Tetromino::moveTo(int row, int col) {
    colOffset += col;
    rowOffset += row;
}

void Tetromino::moveLeft() {
    colOffset -= 1;
}

void Tetromino::moveRight() {
    colOffset += 1;
}

void Tetromino::moveDown() {

    rowOffset += 1;
}

void Tetromino::drop(int maxDrop)
{
    rowOffset += maxDrop;
}

std::vector<Block> Tetromino::getShape() const {
    std::vector<Block> transformed;
    for (const auto& block : rotations[currentRotation]) {
        transformed.push_back({block.row + rowOffset, block.col + colOffset});
    }
    return transformed;
}

QColor Tetromino::getColor() const {
    return color;
}

TetrominoType Tetromino::getType() const
{
    return type;
}
