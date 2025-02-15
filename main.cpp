#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tetrisgame.h"
#include "gridmodel.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Create instances
    GridModel gridmodel;
    TetrisGame tetris(&gridmodel);

    // Expose the objects to QML
    engine.rootContext()->setContextProperty("gridmodel", &gridmodel);
    engine.rootContext()->setContextProperty("tetrisGame", &tetris);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Tetris", "Main");

    return app.exec();
}
