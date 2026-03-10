#pragma once

#include "Core/GlobalEngine.h"

extern std::unique_ptr<Engine::Application> createApplication();

int main(int argc, char** argv)
{
    Engine::GlobalEngine::initialize(std::move(createApplication()));
}
