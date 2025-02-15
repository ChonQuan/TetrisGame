#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QAbstractListModel>
#include <qqmlintegration.h>
#include <QString>
#include <QtQml>
#include <QColor>
#include "tetromino.h"

struct Cell {
    int row;
    int column;
    QColor color;
    bool isClear;
};

class GridModel : public QAbstractListModel {
    Q_OBJECT
public:

    explicit GridModel(QObject *parent = nullptr);

    enum RoleNames {
        Row = Qt::UserRole + 1,
        Column,
        ColorRole,
        IsClear
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setCell(int row, int col, const QColor &color);
    void clearRow(int row);
    void clearGrid();
    bool isOutofBound(int row, int col) const;
    bool isCellUnOccupied(int row, int col) const;
    void setColorTetromino(const std::vector<Block> &blocks, const QColor &color);
    bool Move(const std::vector<Block> &blocks,const QColor &color);
    int calculateScore();
    void shiftDown(int clearedRow);
    void clearPreviousTetromino();
    bool isValidPosition(const std::vector<Block> &blocks);
    int checkMaxDrop(const std::vector<Block> &blocks);
    void printGrid();//test only

private:
    QVector<QVector<Cell>> m_grid;  // 2D Matrix Representation
    std::vector<Block> m_previousShape;
    QColor m_previousTetrominoColor;
};

#endif // GRIDMODEL_H
