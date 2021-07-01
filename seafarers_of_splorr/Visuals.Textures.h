#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
#include "Common.Application.h"
namespace visuals::Textures
{
	const std::shared_ptr<SDL_Texture>& Read(const std::shared_ptr<common::Application::Renderer>&, const std::string&);
}

