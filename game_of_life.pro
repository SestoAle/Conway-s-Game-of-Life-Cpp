TEMPLATE = app
TARGET = game_of_life

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    view.cpp \
    clickablelabel.cpp \
    controller.cpp \
    model.cpp \


HEADERS += \
    view.h \
    clickablelabel.h \
    controller.h \
    model.h \

RESOURCES += \
    img.qrc

