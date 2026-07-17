#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void continueClicked();
    void restartClicked();

private:
    QPushButton *continue_button_;
    QPushButton *restart_button_;
};

#endif // MENUWIDGET_H
