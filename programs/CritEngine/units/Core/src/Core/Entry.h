#pragma once

#include "Core/GlobalEngine.h"

extern std::unique_ptr<Engine::Application> CreateApplication();

int main(int argc, char** argv)
{
	Engine::GlobalEngine::Initialize(std::move(CreateApplication()));
}
