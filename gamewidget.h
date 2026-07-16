#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include "gamemodel.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(GameModel *model, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onGridUpdated(const QList<QPoint>& snake, const QPoint& food);
    void onScoreUpdated(int score);
    void onGameFinished(bool win);

private:
    void drawGrid(QPainter &painter);
    void drawSnake(QPainter &painter);
    void drawFood(QPainter &painter);

    GameModel *model_;
    QList<QPoint> snake_body_;
    QPoint food_;
    int score_ = 0;
    bool is_game_over_ = false;
};

#endif // GAMEWIDGET_H
