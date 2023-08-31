#include "App.hpp"

void App::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void App::initWindow() {}

void App::initVulkan() {}

void App::mainLoop() {}

void App::cleanup() {}
