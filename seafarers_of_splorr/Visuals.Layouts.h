#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
#include "Application.Engine.h"
namespace visuals::Layouts
{
	void Draw(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
	void Start();
}