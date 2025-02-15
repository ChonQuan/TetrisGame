#include "gridmodel.h"

GridModel::GridModel(QObject *parent)
    : QAbstractListModel(parent),
    m_grid(ROWS, QVector<Cell>(COLUMNS, {0, 0, QColor("black"), false})), // Initialize with black color
    m_previousTetrominoColor(QColor("black"))
{
    qDebug() << "Initializing grid with black color...";
}

int GridModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return ROWS * COLUMNS; // Total number of cells in the grid
}

QVariant GridModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    int row = index.row() / COLUMNS;
    int col = index.row() % COLUMNS;

    if (role == Row) {
        return m_grid[row][col].row;
    } else if (role == Column) {
        return m_grid[row][col].column;
    } else if (role == ColorRole) {
        return m_grid[row][col].color.name();
    } else if (role == IsClear){
        return m_grid[row][col].isClear;
    }
    return QVariant();
}

QHash<int, QByteArray> GridModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ColorRole] = "colorCell";
    roles[Row] = "row";
    roles[Column] = "column";
    roles[IsClear] = "isClear";
    return roles;
}

void GridModel::setCell(int row, int col, const QColor &color) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLUMNS) return;

    m_grid[row][col].color = color;

    // Notify QML that the data has changed
    QModelIndex modelIndex = createIndex(row * COLUMNS + col, 0);
    emit dataChanged(modelIndex, modelIndex, {ColorRole});
}

void GridModel::clearRow(int row) {
    if (row < 0 || row >= ROWS) return;

    for (int col = 0; col < COLUMNS; ++col) {
        m_grid[row][col].color = QColor("black"); // Clear row by setting color to black
        m_grid[row][col].isClear = true;
    }

    // Notify QML
    QModelIndex startIndex = createIndex(row * COLUMNS, 0);
    QModelIndex endIndex = createIndex(row * COLUMNS + COLUMNS - 1, 0);
    emit dataChanged(startIndex, endIndex, {IsClear, ColorRole});
}

void GridModel::clearGrid() {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLUMNS; ++col) {
            m_grid[row][col].color = QColor("black");
        }
    }

    // Notify QML that the entire grid changed
    emit dataChanged(createIndex(0, 0), createIndex(ROWS * COLUMNS - 1, 0), {ColorRole});
}

bool GridModel::isOutofBound(int row, int col) const
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLUMNS) {
        return true;
    }
    return false;
}

bool GridModel::isCellUnOccupied(int row, int col) const {
    return m_grid[row][col].color == QColor("black");
}

void GridModel::setColorTetromino(const std::vector<Block> &blocks, const QColor &color)
{
    if (blocks.empty()) return;
    for (const Block& block : blocks){
        setCell(block.row, block.col, color);
    }
}

bool GridModel::Move(const std::vector<Block> &blocks,const QColor &color)
{
    // Check if possible move
    setColorTetromino(m_previousShape, QColor("black"));
    for (const Block& block : blocks){
        if (isOutofBound(block.row, block.col) || isCellUnOccupied(block.row, block.col) == false) {
            setColorTetromino(m_previousShape, m_previousTetrominoColor);
            return false;
        }
    }
    // Update Model
    for (const Block& block : blocks){
        m_grid[block.row][block.col].color = color;
    }

    m_previousShape = blocks;
    m_previousTetrominoColor = color;

    // printGrid();
    // Notify QML that the entire grid changed
    emit dataChanged(createIndex(0, 0), createIndex(ROWS * COLUMNS - 1, 0), {ColorRole});
    return true;
}

int GridModel::calculateScore() {
    int rowsCleared = 0;
    for (int row = 0; row < ROWS; ++row) {
        bool fullRow = true;
        for (int col = 0; col < COLUMNS; ++col) {
            if (m_grid[row][col].color == QColor("black")) {
                fullRow = false;
                break;
            }
        }

        if (fullRow) {
            rowsCleared++;
            clearRow(row);  // Clear the row

            QEventLoop loop;
            QTimer::singleShot(500, &loop, &QEventLoop::quit);  // Pause for 500ms
            loop.exec();  // Blocks execution but keeps the UI responsive

            shiftDown(row); // Move everything above it down
        }
    }

    // Score system: 10 points per row, bonus for multiple rows
    int score = 0;
    if (rowsCleared > 0) {
        score += rowsCleared * 10;
    }

    return score;
}

void GridModel::shiftDown(int clearedRow) {
    for (int row = clearedRow; row > 0; --row) {
        std::swap(m_grid[row], m_grid[row - 1]);  // Swap rows instead of copying elements
    }

    // Clear the top row
    std::fill(m_grid[0].begin(), m_grid[0].end(), Cell{0, 0, QColor("black"),false});

    emit dataChanged(createIndex(0, 0), createIndex(ROWS * COLUMNS - 1, 0), {IsClear, ColorRole});
}

void GridModel::clearPreviousTetromino()
{
    m_previousShape.clear();
    m_previousTetrominoColor = QColor("black");
}

bool GridModel::isValidPosition(const std::vector<Block> &blocks)
{
    for (const Block& block : blocks){
        if (isCellUnOccupied(block.row, block.col) == false) {
            return false;
        }
    }
    return true;
}

int GridModel::checkMaxDrop(const std::vector<Block> &blocks) {
    int maxDrop = ROWS;  // Start with the maximum possible drop

    // Temporarily remove the tetromino from the grid (mark its cells as empty)
    setColorTetromino(blocks, QColor("black"));

    for (const Block& block : blocks) {
        int dropHeight = 0;
        int row = block.row;

        // Keep moving downward until we hit an occupied cell or the bottom
        while (row + dropHeight < ROWS && isCellUnOccupied(row + dropHeight, block.col)) {
            dropHeight++;
        }

        // Update maxDrop with the lowest drop possible for all blocks
        maxDrop = std::min(maxDrop, dropHeight - 1);
    }

    // Restore the tetromino's color in the grid
    setColorTetromino(blocks, m_previousTetrominoColor);

    return maxDrop; // Now it correctly returns how many rows the tetromino can drop
}

void GridModel::printGrid() {
    for (int row = 0; row < ROWS; ++row) {
        QString rowString;
        for (int col = 0; col < COLUMNS; ++col) {
            rowString += (m_grid[row][col].color == QColor("black")) ? "0 " : "1 ";
        }
        qDebug() << rowString;
    }
}

