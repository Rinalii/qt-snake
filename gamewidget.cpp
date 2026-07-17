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

    // Создаём меню
    menu_widget_ = new MenuWidget(this);
    menu_widget_->hide(); // по умолчанию скрыто

    connect(menu_widget_, &MenuWidget::continueClicked, this, &GameWidget::TogglePause);
    connect(menu_widget_, &MenuWidget::restartClicked,
            [this]() {
                model_->Restart();
                // Если игра была на паузе, выключаем паузу
                if (is_paused_) {
                    TogglePause();
                }
                // Сбрасываем флаг окончания игры
                is_game_over_ = false;
                update();
            });

    // Создаём лейбл для счёта
    score_label = new QLabel(this);
    score_label->setText("Счёт: 0");
    score_label->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        "background: rgba(0, 0, 0, 150);"
        "padding: 5px 15px;"
        "border-radius: 10px;"
        );

    score_label->adjustSize();

    int margin = 10;
    int x = width() - score_label->width() - margin;
    int y = margin;
    score_label->move(x, y);
    // лейбл поверх всего
    score_label->raise();
}

void GameWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    painter.fillRect(rect(), QColor("#3f7356"));

    DrawGrid(painter);
    DrawFood(painter);
    DrawSnake(painter);

    if (is_game_over_) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, QString("GAME OVER\nSCORE: %1").arg(score_));
    }
}

void GameWidget::DrawGrid(QPainter &painter) {
    painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));
    const int cell = GameConfig::Field::CELL_SIZE;
    for (int x = 0; x < GameConfig::Field::WIDTH; ++x) {
        for (int y = 0; y < GameConfig::Field::HEIGHT; ++y) {
            if((x+y)%2==0) {
                QRect curr_rect(x*cell, y*cell, cell, cell);
                painter.fillRect(curr_rect, QColor("#386646"));
            }
        }
    }
}

void GameWidget::DrawSegment(QPainter &painter, const QPoint &point, int n, int curr_num, QColor color) {
    const int cell = GameConfig::Field::CELL_SIZE;
    // Коэфф. уменьш.: голова – 1.0, хвост – 0.3
    float scale = 1.0f - 0.7f * (curr_num / float(n - 1));
    int size = qRound(cell * scale);
    // Центр ячейки
    int cx = point.x() * cell + cell / 2;
    int cy = point.y() * cell + cell / 2;
    QRect rect(cx - size/2, cy - size/2, size, size);

    painter.setBrush(color);
    painter.drawRoundedRect(rect, size/4.0, size/4.0);
}

void GameWidget::DrawEyes(QPainter &painter, const QPoint &point, Snake::Direction dir) {
    const int cell = GameConfig::Field::CELL_SIZE;
    int size = qRound(cell * 0.2);

    if(dir == Snake::Direction::UP) {
        int cx = point.x() * cell + cell / 5;
        int cy = point.y() * cell + cell / 5;
        painter.setBrush(Qt::black);
        painter.drawEllipse(cx, cy, size, size);

        cx = point.x() * cell + 3*cell / 5;
        cy = point.y() * cell + cell / 5;
        painter.drawEllipse(cx, cy, size, size);
    } else if (dir == Snake::Direction::LEFT) {
        int cx = point.x() * cell + cell / 5;
        int cy = point.y() * cell + cell / 5;
        painter.setBrush(Qt::black);
        painter.drawEllipse(cx, cy, size, size);

        cx = point.x() * cell + cell / 5;
        cy = point.y() * cell + 3*cell / 5;
        painter.drawEllipse(cx, cy, size, size);
    } else if (dir == Snake::Direction::RIGHT) {
        int cx = point.x() * cell + 3*cell / 5;
        int cy = point.y() * cell + cell / 5;
        painter.setBrush(Qt::black);
        painter.drawEllipse(cx, cy, size, size);

        cx = point.x() * cell + 3*cell / 5;
        cy = point.y() * cell + 3*cell / 5;
        painter.drawEllipse(cx, cy, size, size);
    } else {
        int cx = point.x() * cell + cell / 5;
        int cy = point.y() * cell + 3*cell / 5;
        painter.setBrush(Qt::black);
        painter.drawEllipse(cx, cy, size, size);

        cx = point.x() * cell + 3*cell / 5;
        cy = point.y() * cell + 3*cell / 5;
        painter.drawEllipse(cx, cy, size, size);
    }
}

void GameWidget::DrawSnake(QPainter &painter) {
    if (snake_body_.isEmpty()) return;
    painter.setPen(Qt::NoPen);
    const int n = snake_body_.size();

    DrawSegment(painter, snake_body_[0], n, 0, Qt::green);
    DrawEyes(painter, snake_body_[0], snake_dir_);


    for (int i = 1; i < n; ++i) {
        DrawSegment(painter, snake_body_[i], n, i, Qt::green);
    }
}

void GameWidget::DrawFood(QPainter &painter) {
    const int cell = GameConfig::Field::CELL_SIZE;
    QRect foodRect(food_.x() * cell, food_.y() * cell, cell, cell);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(foodRect);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {
        if (!is_game_over_) {
            TogglePause();
        }
        return;
    }

    // Если меню открыто или игра окончена, игнорируем управление змейкой
    if (menu_widget_->isVisible() || is_game_over_) {
        QWidget::keyPressEvent(event);
        return;
    }

    // Обработка стрелок
    Snake::Direction dir = Snake::Direction::UP;
    switch (event->key()) {
    case Qt::Key_Up:    dir = Snake::Direction::UP;    break;
    case Qt::Key_Down:  dir = Snake::Direction::DOWN;  break;
    case Qt::Key_Left:  dir = Snake::Direction::LEFT;  break;
    case Qt::Key_Right: dir = Snake::Direction::RIGHT; break;
    default: QWidget::keyPressEvent(event); return;
    }
    model_->SetDirection(dir);
}

void GameWidget::onGridUpdated(const QList<QPoint>& snake, Snake::Direction dir, const QPoint& food) {
    snake_body_ = snake;
    snake_dir_ = dir;
    food_ = food;
    update();
}

void GameWidget::onScoreUpdated(int score) {
    score_ = score;
    if (score_label) {
        score_label->setText("Счёт: " + QString::number(score));
        score_label->adjustSize();

        int margin = 10;
        int x = width() - score_label->width() - margin;
        int y = margin;
        score_label->move(x, y);
    }
}

void GameWidget::onGameFinished(bool win) {
    is_game_over_ = true;
    menu_widget_->hide();
    update();

    QMessageBox::information(this, "Игра окончена",
                             win ? "Вы победили!" : "Вы проиграли :(",
                             QMessageBox::Ok);

    model_->Restart();
    is_game_over_ = false;
    update();
}

void GameWidget::TogglePause() {
    if (is_game_over_) return;

    is_paused_ = !is_paused_;
    if (is_paused_) {
        model_->Pause();
        menu_widget_->show();
    } else {
        model_->Resume();
        menu_widget_->hide();
        setFocus();
    }
}

void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (menu_widget_) {
        // Центрируем меню по центру игрового виджета
        int x = (width() - menu_widget_->width()) / 2;
        int y = (height() - menu_widget_->height()) / 2;
        menu_widget_->move(x, y);
    }
}
