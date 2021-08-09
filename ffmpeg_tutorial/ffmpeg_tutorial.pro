TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


include(../ffmpeg_include.pri)

include(../sdl2_config.pri)

SOURCES += \
#        02_avformat_context.c \
#        03_avcodec_context.c \
#        04_decode_and_dump_video.c \
        05_decode_and_display_video.c
#        a11_extract_audio.c
