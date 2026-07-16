#include "mainwindow.h"
#include "gamemodel.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    m_model = new GameModel(this);
    m_widget = new GameWidget(m_model, this);

    setCentralWidget(m_widget);
    setWindowTitle("Змейка");
    adjustSize();
}

MainWindow::~MainWindow() = default;
