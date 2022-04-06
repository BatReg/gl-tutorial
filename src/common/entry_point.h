#pragma once

#include "application.h"

Application* CreateApplication();

int main(int argc, char** argv)
{
    auto app = CreateApplication();
    int returnCode = app->Run();
    delete app;

    return returnCode;
}