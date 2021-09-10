#include <iostream>
#include "application.h"
#undef main

int main(int argc, char *argv[])
{
    Application app;

    app.loop();
}
