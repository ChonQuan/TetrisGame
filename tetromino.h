#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <QColor>
#include <QDebug>

enum class TetrominoType { I, O, T, S, Z, J, L };

struct Block {
    int row;
    int col;
};

static constexpr int COLUMNS = 10;
static constexpr int ROWS = 20;

class Tetromino {
public:
    virtual ~Tetromino() = default;

    void rotate();
    void unRotate();
    void moveTo(int row, int col);
    void moveLeft();
    void moveRight();
    void moveDown();
    void drop(int maxDrop);

    std::vector<Block> getShape() const;
    QColor getColor() const;
    TetrominoType getType() const;

protected:
    std::vector<std::vector<Block>> rotations;
    int currentRotation = 0;
    QColor color;
    int rowOffset = 0, colOffset = 0;
    TetrominoType type;
};

class TetrominoI : public Tetromino {
public:
    TetrominoI() {
        type = TetrominoType::I;
        rotations = {
            {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
            {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
            {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
            {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
        };
        color = QColor("cyan");
        moveTo(0,2);
    }
};

class TetrominoO : public Tetromino {
public:
    TetrominoO() {
        type = TetrominoType::O;
        rotations = {
            {{0, 1}, {0, 2}, {1, 1}, {1, 2}}
        };
        color = QColor("yellow");
        moveTo(0,3);
    }
};

class TetrominoT : public Tetromino {
public:
    TetrominoT() {
        type = TetrominoType::T;
        rotations = {
            {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
            {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
            {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
            {{0, 1}, {1, 0}, {1, 1}, {2, 1}}
        };
        color = QColor("magenta");
        moveTo(0,3);
    }
};

class TetrominoS : public Tetromino {
public:
    TetrominoS() {
        type = TetrominoType::S;
        rotations = {
            {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
            {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
            {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
        };
        color = QColor("green");
        moveTo(0,3);
    }
};
class TetrominoZ : public Tetromino {
public:
    TetrominoZ() {
        type = TetrominoType::Z;
        rotations = {
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
            {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
            {{0, 1}, {1, 0}, {1, 1}, {2, 0}}
        };
        color = QColor("red");
        moveTo(0,3);
    }
};
class TetrominoJ : public Tetromino {
public:
    TetrominoJ() {
        type = TetrominoType::J;
        rotations = {
            {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
            {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
            {{0, 1}, {1, 1}, {2, 0}, {2, 1}}
        };
        color = QColor("blue");
        moveTo(0,3);
    }
};
class TetrominoL : public Tetromino {
public:
    TetrominoL() {
        type = TetrominoType::L;
        rotations = {
            {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
            {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
            {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
            {{0, 0}, {0, 1}, {1, 1}, {2, 1}}
        };
        color = QColor("orange");
        moveTo(0,3);
    }
};

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

#endif // TETROMINO_H
