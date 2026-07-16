#include "snake.h"
#include "gamesettings.h"

Snake::Snake() {
    direction_=Direction::LEFT;
    for(int i = 0; i < GameConfig::Snake::INITIAL_SIZE; ++i) {
        auto& back = body_.emplaceBack(i, 0);
        occupied_.insert(back);
    }
}

void Snake::Restart() {
    direction_=Direction::LEFT;
    body_.clear();
    occupied_.clear();

    for(int i = 0; i < GameConfig::Snake::INITIAL_SIZE; ++i) {
        auto& back = body_.emplaceBack(i, 0);
        occupied_.insert(back);
    }
}

int Snake::GetSnakeSize() const {
    return body_.size();
}

const QList<QPoint>& Snake::GetBody() const {
    return body_;
}

bool Snake::IsValidDirection(Direction direction) const {
    switch(direction) {
    case Direction::DOWN:
        if(direction_ == Direction::UP) {
            return false;
        }
        return true;
    case Direction::LEFT:
        if(direction_ == Direction::RIGHT) {
            return false;
        }
        return true;
    case Direction::RIGHT:
        if(direction_ == Direction::LEFT) {
            return false;
        }
        return true;
    case Direction::UP:
        if(direction_ == Direction::DOWN) {
            return false;
        }
        return true;
    default:
        return true;
    }
}

void Snake::SetDirection(Direction direction) {
    if(IsValidDirection(direction)) {
        direction_ = direction;
    }
}

QPoint Snake::GetHeadNextCoordinate() const {
    const QPoint& curr_coord = body_.first();
    switch(direction_) {
    case Direction::RIGHT:
        if(curr_coord.x() >= GameConfig::Field::WIDTH-1) {
            return QPoint(0, curr_coord.y());
        }
        return QPoint(curr_coord.x()+1, curr_coord.y());
    case Direction::LEFT:
        if(curr_coord.x() <= 0) {
            return QPoint(GameConfig::Field::WIDTH-1, curr_coord.y());
        }
        return QPoint(curr_coord.x()-1, curr_coord.y());
    case Direction::UP:
        if(curr_coord.y() <= 0) {
            return QPoint(curr_coord.x(), GameConfig::Field::HEIGHT-1);
        }
        return QPoint(curr_coord.x(), curr_coord.y()-1);
    default:
        if(curr_coord.y() >= GameConfig::Field::HEIGHT-1) {
            return QPoint(curr_coord.x(), 0);
        }
        return QPoint(curr_coord.x(), curr_coord.y()+1);
    }
}

bool Snake::IsOccupied(QPoint coordinate) const {
    return occupied_.contains(coordinate);
}

void Snake::MoveToFoodCell(QPoint coordinate) {
    body_.push_front(coordinate);
    occupied_.insert(coordinate);
}

void Snake::MoveToRegularCell(QPoint coordinate) {
    body_.push_front(coordinate);
    occupied_.insert(coordinate);

    occupied_.remove(body_.last());
    body_.removeLast();
}
