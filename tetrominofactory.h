#ifndef TETROMINOFACTORY_H
#define TETROMINOFACTORY_H

#include "tetromino.h"

class TetrominoFactory {
public:
    static Tetromino* createTetromino(TetrominoType type) {
        switch (type) {
        case TetrominoType::I: return new TetrominoI();
        case TetrominoType::O: return new TetrominoO();
        case TetrominoType::T: return new TetrominoT();
        case TetrominoType::S: return new TetrominoS();
        case TetrominoType::Z: return new TetrominoZ();
        case TetrominoType::J: return new TetrominoJ();
        case TetrominoType::L: return new TetrominoL();
        default: return nullptr;
        }
    }
};

#endif // TETROMINOFACTORY_H
