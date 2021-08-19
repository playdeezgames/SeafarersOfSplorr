#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
#include "Application.Engine.h"
#include "Visuals.DrawerFunction.h"
namespace visuals::Layouts
{
	void RegisterType(const std::string&, InternalizerFunction);
	std::function<void()> DoRegisterType(const std::string&, InternalizerFunction);
	void Draw(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
	void Start();
}