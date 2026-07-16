#include "gamewidget.h"
#include "gamesettings.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>

GameWidget::GameWidget(GameModel *model, QWidget *parent)
    : QWidget(parent)
    , model_(model) {
    setFixedSize(GameConfig::Field::WIDTH  * GameConfig::Field::CELL_SIZE,
                 GameConfig::Field::HEIGHT * GameConfig::Field::CELL_SIZE);

    // Подписываемся на сигналы модели
    connect(model_, &GameModel::GridUpdated, this, &GameWidget::onGridUpdated);
    connect(model_, &GameModel::ScoreUpdated, this, &GameWidget::onScoreUpdated);
    connect(model_, &GameModel::GameFinished, this, &GameWidget::onGameFinished);

    // Фокус для получения клавиатурных событий
    setFocusPolicy(Qt::StrongFocus);
}

void GameWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    drawGrid(painter);
    drawFood(painter);
    drawSnake(painter);

    if (is_game_over_) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    }
}

void GameWidget::drawGrid(QPainter &painter) {
    painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
    const int cell = GameConfig::Field::CELL_SIZE;
    for (int x = 0; x <= GameConfig::Field::WIDTH; ++x) {
        painter.drawLine(x * cell, 0, x * cell, height());
    }
    for (int y = 0; y <= GameConfig::Field::HEIGHT; ++y) {
        painter.drawLine(0, y * cell, width(), y * cell);
    }
}

void GameWidget::drawSnake(QPainter &painter) {
    if (snake_body_.isEmpty()) return;
    const int cell = GameConfig::Field::CELL_SIZE;
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < snake_body_.size(); ++i) {
        const QPoint &p = snake_body_[i];
        QRect cellRect(p.x() * cell, p.y() * cell, cell, cell);
        if (i == 0) {
            painter.setBrush(Qt::yellow);
        } else {
            painter.setBrush(Qt::green);
        }
        painter.drawRect(cellRect);
    }
}

void GameWidget::drawFood(QPainter &painter) {
    const int cell = GameConfig::Field::CELL_SIZE;
    QRect foodRect(food_.x() * cell, food_.y() * cell, cell, cell);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(foodRect);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    if (is_game_over_) {
        QWidget::keyPressEvent(event);
        return;
    }

    Snake::Direction dir = Snake::Direction::UP;
    switch (event->key()) {
    case Qt::Key_Up:    dir = Snake::Direction::UP;    break;
    case Qt::Key_Down:  dir = Snake::Direction::DOWN;  break;
    case Qt::Key_Left:  dir = Snake::Direction::LEFT;  break;
    case Qt::Key_Right: dir = Snake::Direction::RIGHT; break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    model_->SetDirection(dir);
}

void GameWidget::onGridUpdated(const QList<QPoint>& snake, const QPoint& food) {
    snake_body_ = snake;
    food_ = food;
    update();
}

void GameWidget::onScoreUpdated(int score) {
    score_ = score;
    setWindowTitle(QString("Змейка - Счёт: %1").arg(score));
}

void GameWidget::onGameFinished(bool win) {
    is_game_over_ = true;
    update();

    QMessageBox::information(this, "Игра окончена",
                             win ? "Вы победили!" : "Вы проиграли :(",
                             QMessageBox::Ok);

    model_->Restart();
    is_game_over_ = false;
    update();
}
