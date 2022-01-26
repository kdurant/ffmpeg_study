FFMPEG_PATH = D:/Lib/ffmpeg
#
INCLUDEPATH += $$FFMPEG_PATH/include
LIBS += -L$$FFMPEG_PATH/lib -lavcodec    \
        -L$$FFMPEG_PATH/lib -lavdevice   \
        -L$$FFMPEG_PATH/lib -lavfilter   \
        -L$$FFMPEG_PATH/lib -lavformat   \
        -L$$FFMPEG_PATH/lib -lavutil     \
        -L$$FFMPEG_PATH/lib -lswresample \
        -L$$FFMPEG_PATH/lib -lswscale

#message($$PWD)
#win32:CONFIG(release, debug | release)
#{
#    LibFile = $$FFMPEG_PATH/bin/*.dll
#    LibFile = $$replace(LibFile, /, \\)
#    OutLibFile = $${OUT_PWD}/debug/*.dll
#    OutLibFile = $$replace(OutLibFile, /, \\)
#    QMAKE_PRE_LINK += copy  $$LibFile $$OutLibFile
#}

defineTest(copyToDestdir) {
    files = $$1
    DDIR = $$2

    for(FILE, files) {
        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_PRE_LINK)
}

copyToDestdir($$FFMPEG_PATH/bin/avcodec-59.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/avdevice-59.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/avfilter-8.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/avformat-59.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/avutil-57.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/postproc-56.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/swresample-4.dll, $${OUT_PWD}/debug)
copyToDestdir($$FFMPEG_PATH/bin/swscale-6.dll, $${OUT_PWD}/debug)


