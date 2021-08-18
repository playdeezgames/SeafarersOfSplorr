#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
#include "Application.Engine.h"
namespace visuals::Textures
{
	const std::shared_ptr<SDL_Texture>& Read(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
}

