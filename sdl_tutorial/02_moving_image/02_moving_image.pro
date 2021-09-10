TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(../../sdl2_config.pri)

SOURCES += \
    application.cpp \
    main.cpp

DISTFILES += \
    stick_figure.bmp

HEADERS += \
    application.h
