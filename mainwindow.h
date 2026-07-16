#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameModel;
class GameWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    GameModel *m_model;
    GameWidget *m_widget;
};

#endif // MAINWINDOW_H
