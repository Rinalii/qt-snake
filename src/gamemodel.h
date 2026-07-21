#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>

#include "snake.h"

class GameModel : public QObject {
    Q_OBJECT
public:
    GameModel(QObject *parent = nullptr);
    void GenerateFood();
    void SetDirection(Snake::Direction dir);
    void Restart();
    void Pause();
    void Resume();
    void TogglePause();
    void Start();
signals:
    void GridUpdated(const QList<QPoint>& snake, Snake::Direction dir, const QPoint& food);
    void ScoreUpdated(int score);
    void GameFinished(bool win);
private slots:
    void MoveSnakeSlot();
private:
    QTimer timer_;
    std::unique_ptr<Snake> snake_;
    bool is_move_blocked_;
    QPoint food_;
    int score_{0};
};

#endif // GAMEMODEL_H
