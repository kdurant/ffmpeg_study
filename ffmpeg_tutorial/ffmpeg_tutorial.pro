TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


include(../ffmpeg_include.pri)


SOURCES += \
#        04_decode_and_dump_video.c
        a11_extract_audio.c
