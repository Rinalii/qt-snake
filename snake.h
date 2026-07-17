#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <QSet>
#include <QPoint>

class Snake {
public:
    enum class Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    Snake();
    void Restart();
    int GetSnakeSize() const;
    const QList<QPoint>& GetBody() const;

    bool IsValidDirection(Direction direction) const;
    void SetDirection(Direction direction);
    Direction GetDirection() const;

    QPoint GetHeadNextCoordinate() const;

    bool IsOccupied(QPoint coordinate) const;

    void MoveToFoodCell(QPoint coordinate);
    void MoveToRegularCell(QPoint coordinate);

private:
    QList<QPoint> body_;
    QSet<QPoint> occupied_;
    Direction direction_;
};

#endif // SNAKE_H
