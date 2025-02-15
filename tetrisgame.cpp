#include "tetrisgame.h"
#include <QDebug>

TetrisGame::TetrisGame(GridModel* gridmodel, QObject *parent)
    : QObject(parent), m_score(0), m_gameOver(true), m_pauseGame(false), m_firstTime(true) {
    // Initialize grid and game state
    m_gridmodel = gridmodel;

    m_timer = new QTimer(this);
    m_currentTetromino = nullptr;
    m_nextTetromino = nullptr;
    timer = 1000;
    // Connect timer's timeout signal to moveDown function
    connect(m_timer, &QTimer::timeout, this, &TetrisGame::moveDown);
}

int TetrisGame::score() const {
    return m_score;
}

int TetrisGame::firstTime() const
{
    return m_firstTime;
}

int TetrisGame::gameOver() const
{
    return m_gameOver;
}

int TetrisGame::pauseGame() const
{
    return m_pauseGame;
}

void TetrisGame::moveLeft() {
    if (!m_gameOver) {
        m_currentTetromino->moveLeft();
        bool checkMoveFinish = m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
        //Check moving is finished
        if (!checkMoveFinish) m_currentTetromino->moveRight();
    }
}

void TetrisGame::moveRight() {
    if (!m_gameOver) {
        m_currentTetromino->moveRight();
        bool checkMoveFinish = m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
        //Check moving is finished
        if (!checkMoveFinish) m_currentTetromino->moveLeft();
    }
}

void TetrisGame::moveDown()
{
    if (!m_gameOver) {
        m_currentTetromino->moveDown();
        bool checkMoveFinish = m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
        //Check moving is finished
        if (!checkMoveFinish) {
            m_gridmodel->clearPreviousTetromino();
            updateScore();
            lockTetromino();
        }
    } else {
        m_timer->stop();
    }
}

void TetrisGame::rotate() {
    if (!m_gameOver) {
        m_currentTetromino->Rotate();
        bool checkMoveFinish = m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
        //Check moving is finished
        if (!checkMoveFinish) m_currentTetromino->unRotate();
    }
}

void TetrisGame::drop() {
    if (!m_gameOver) {
        m_timer->stop();
        int maxDrop = m_gridmodel->checkMaxDrop(m_currentTetromino->getShape());
        m_currentTetromino->drop(maxDrop);
        bool checkMoveFinish = m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
        moveDown();
        m_timer->start(timer);
    }
}

void TetrisGame::reset() {

    if (!m_gridmodel) return;  // Prevent crashes if model is null

    // Clear the grid
    m_gridmodel->clearGrid();

    // Delete existing Tetrominoes safely
    if (m_currentTetromino) {
        delete m_currentTetromino;
        m_currentTetromino = nullptr;
    }

    if (m_nextTetromino) {
        qDebug() << "delete" ;
        delete m_nextTetromino;
        m_nextTetromino = nullptr;
    }

    // Create new Tetrominoes
    m_currentTetromino = getRamdomTetro();
    m_nextTetromino = getRamdomTetro();
    int type = static_cast<int>(m_nextTetromino->getType());
    emit newTetrominoChanged(type);

    // Ensure Tetromino is placed in the grid
    if (m_currentTetromino) {
        m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
    }

    // Reset game variables
    updateGameOver(false);
    updatePauseGame(false);
    updateFirstTime(false);
    m_score = 0;
    emit scoreChanged();

    // Restart timer for movement
    if (m_timer) {
        m_timer->stop();
        m_timer->start(timer);  // Adjust speed as needed
    }
}

void TetrisGame::pause() {
    updatePauseGame(true);
    m_timer->stop();  // Stop the timer to halt movement
}

void TetrisGame::resume() {
    updatePauseGame(false);
    m_timer->start(timer);
}

void TetrisGame::endGame() {
    qDebug() << "endGame" ;
    updateGameOver(true);
    m_timer->stop();  // Stop the timer to halt movement
}

void TetrisGame::updateScore()
{
    m_timer->stop();
    m_score += m_gridmodel->calculateScore();
    emit scoreChanged();
    m_timer->start();
}

void TetrisGame::updateFirstTime(const bool &firstTime)
{
    if (m_firstTime != firstTime){
        m_firstTime = firstTime;
        emit firstTimeChanged();
    }
}

void TetrisGame::updateGameOver(const bool &gameover)
{
    if (m_gameOver != gameover){
        m_gameOver = gameover;
        emit gameOverChanged();
    }
}

void TetrisGame::updatePauseGame(const bool &pauseGame)
{
    if (m_pauseGame != pauseGame){
        m_pauseGame = pauseGame;
        emit pauseGameChanged();
    }
}

Tetromino* TetrisGame::getRamdomTetro()
{
    TetrominoType randomType = static_cast<TetrominoType>(QRandomGenerator::global()->bounded(7));
    return TetrominoFactory::createTetromino(randomType);
}

void TetrisGame::lockTetromino()
{
    // Delete the old Tetromino to free memory
    delete m_currentTetromino;
    m_currentTetromino = nullptr;

    // Spawn a new Tetromino
    m_currentTetromino = m_nextTetromino;
    m_nextTetromino = getRamdomTetro();

    int type = static_cast<int>(m_nextTetromino->getType());
    emit newTetrominoChanged(type);

    // check game over
    if (m_gridmodel->isValidPosition(m_currentTetromino->getShape())){
        m_gridmodel->Move(m_currentTetromino->getShape(), m_currentTetromino->getColor());
    } else {
        endGame();
    }
}
