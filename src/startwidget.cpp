#include "startwidget.h"

#include <Qlabel>
#include <QVBoxLayout>

StartWidget::StartWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color: rgba(0, 0, 0, 180);");

    QLabel *title = new QLabel("ЗМЕЙКА", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; font-size: 28px; font-weight: bold;");


    name_ = new QLineEdit(this);
    name_->setPlaceholderText("Введите имя");
    start_button_ = new QPushButton("Старт", this);

    name_->setStyleSheet("font-size: 16px; padding: 8px;");
    start_button_->setStyleSheet("font-size: 16px; padding: 8px;");

    connect(name_, &QLineEdit::returnPressed, this, &StartWidget::startClicked);
    connect(start_button_, &QPushButton::clicked, this, &StartWidget::startClicked);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(20);
    layout->addWidget(name_);
    layout->addWidget(start_button_);
    layout->addStretch();
    setLayout(layout);

    setFixedSize(300, 200);
}
