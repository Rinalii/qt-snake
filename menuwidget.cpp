#include "menuwidget.h"
#include <QLabel>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: rgba(0, 0, 0, 180);");

    QLabel *title = new QLabel("ПАУЗА", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold;");

    continue_button_ = new QPushButton("Продолжить", this);
    restart_button_ = new QPushButton("Сброс", this);

    continue_button_->setStyleSheet("font-size: 16px; padding: 8px;");
    restart_button_->setStyleSheet("font-size: 16px; padding: 8px;");

    connect(continue_button_, &QPushButton::clicked, this, &MenuWidget::continueClicked);
    connect(restart_button_, &QPushButton::clicked, this, &MenuWidget::restartClicked);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(20);
    layout->addWidget(continue_button_);
    layout->addWidget(restart_button_);
    layout->addStretch();
    setLayout(layout);

    setFixedSize(300, 200);
}
