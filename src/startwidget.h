#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class StartWidget : public QWidget {
    Q_OBJECT
public:
    explicit StartWidget(QWidget *parent = nullptr);

signals:
    void startClicked();

private:
    QLineEdit *name_;
    QPushButton *start_button_;
};

#endif // STARTWIDGET_H
