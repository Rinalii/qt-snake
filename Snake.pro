QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/gamemodel.cpp \
    src/gamewidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/menuwidget.cpp \
    src/snake.cpp \
    src/startwidget.cpp

HEADERS += \
    src/gamemodel.h \
    src/gamesettings.h \
    src/gamewidget.h \
    src/mainwindow.h \
    src/menuwidget.h \
    src/snake.h \
    src/startwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
