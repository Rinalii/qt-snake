#include "gamemodel.h"

#include <QRandomGenerator>

#include "gamesettings.h"

GameModel::GameModel(QObject *parent)
    :   QObject(parent),
    timer_(this) {

    snake_ = std::make_unique<Snake>();
    is_move_blocked_ = false;
    timer_.setInterval(GameConfig::Timing::INITIAL_INTERVAL_MS);
    connect(&timer_, &QTimer::timeout, this, &GameModel::MoveSnakeSlot);
    timer_.start();
}

void GameModel::GenerateFood() {
    food_.setX(QRandomGenerator::global()->bounded(0, GameConfig::Field::WIDTH - 1));
    food_.setY(QRandomGenerator::global()->bounded(0, GameConfig::Field::HEIGHT - 1));
    if(snake_->IsOccupied(food_)) {
        return GenerateFood();
    }
}

void GameModel::SetDirection(Snake::Direction dir) {
    if(is_move_blocked_) {
        return;
    }
    snake_->SetDirection(dir);
    is_move_blocked_ = true;
}

void GameModel::Restart() {
    timer_.stop();
    snake_->Restart();
    is_move_blocked_ = false;
    GenerateFood();
    score_ = 0;
    timer_.start();
    emit GridUpdated(snake_->GetBody(), snake_->GetDirection(), food_);
    emit ScoreUpdated(score_);
}

void GameModel::MoveSnakeSlot() {
    const QPoint& next_coord = snake_->GetHeadNextCoordinate();
    if(food_ == next_coord) {
        snake_->MoveToFoodCell(next_coord);
        score_ += GameConfig::Food::REWARD;
        emit ScoreUpdated(score_);
        GenerateFood();
    } else {
        if(snake_->IsOccupied(next_coord) && snake_->GetBody().back() != next_coord) {
            emit GameFinished(false);
        } else {
            snake_->MoveToRegularCell(next_coord);
        }
    }
    emit GridUpdated(snake_->GetBody(), snake_->GetDirection(), food_);
    is_move_blocked_ = false;
}

void GameModel::Pause() {
    timer_.stop();
}

void GameModel::Resume() {
    timer_.start();
}

void GameModel::TogglePause() {
    if (timer_.isActive()) {
        timer_.stop();
    } else {
        timer_.start();
    }
}
