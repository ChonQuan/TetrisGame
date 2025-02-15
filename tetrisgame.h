#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <QObject>
#include <gridmodel.h>
#include <tetromino.h>
#include <QRandomGenerator>

class TetrisGame : public QObject {
    Q_OBJECT
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(int gameOver READ gameOver NOTIFY gameOverChanged)
    Q_PROPERTY(int pauseGame READ pauseGame NOTIFY pauseGameChanged)

public:
    explicit TetrisGame(GridModel* gridmodel, QObject *parent = nullptr);

    int score() const;

    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void moveDown();
    Q_INVOKABLE void rotate();
    Q_INVOKABLE void drop();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void endGame();

signals:
    void scoreChanged();
    void gameOverChanged();
    void pauseGameChanged();
    void newTetrominoChanged(int type);

public:
    void updateScore();
    void updateGameOver(const bool &gameover);
    void updatePauseGame(const bool &pauseGame);
    Tetromino* getRamdomTetro();
    void lockTetromino();

    int gameOver() const;

    int pauseGame() const;

private:
    int m_score;
    bool m_pauseGame;
    bool m_gameOver;
    GridModel* m_gridmodel;
    Tetromino* m_currentTetromino;
    Tetromino* m_nextTetromino;
    QTimer *m_timer;
    int timer;
};

#endif // TETRISGAME_H

