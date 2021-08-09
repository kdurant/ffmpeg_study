TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include(../sdl2_config.pri)


#SOURCES += \
#    01_hello_world.c
#        02_draw_bmp_image.cpp \
#        ./basic_window/main.cpp \

#include($$PWD/basic_window/basic_window.pri)
include($$PWD/02_moving_image/moving_image.pri)
