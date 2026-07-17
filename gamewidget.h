#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "gamemodel.h"

#include "menuwidget.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(GameModel *model, QWidget *parent = nullptr);
    void TogglePause();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override; // для центрирования меню

private slots:
    void onGridUpdated(const QList<QPoint>& snake, Snake::Direction dir, const QPoint& food);
    void onScoreUpdated(int score);
    void onGameFinished(bool win);

private:
    void DrawSegment(QPainter &painter, const QPoint &point, int n, int curr_num, QColor color);
    void DrawEyes(QPainter &painter, const QPoint &point, Snake::Direction dir);

    void DrawGrid(QPainter &painter);
    void DrawSnake(QPainter &painter);
    void DrawFood(QPainter &painter);

    GameModel *model_;
    MenuWidget *menu_widget_;
    QList<QPoint> snake_body_;
    Snake::Direction snake_dir_;
    QPoint food_;
    int score_ = 0;
    bool is_game_over_ = false;
    bool is_paused_ = false;
};

#endif // GAMEWIDGET_H
