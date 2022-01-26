SDL2_PATH = D:/Lib/SDL2

INCLUDEPATH += $$SDL2_PATH/include

LIBS += -L$$SDL2_PATH/lib/x64 -lSDL2
LIBS += -L$$SDL2_PATH/lib/x64 -lSDL2main
LIBS += -L$$SDL2_PATH/lib/x64 -lSDL2test

# #undef main

