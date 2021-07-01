#pragma once
#include <string>
#include <SDL.h>
#include <memory>
#include "json.hpp"
#include <vector>
#include "Common.Application.h"
namespace visuals::Layouts
{
	void Draw(const std::shared_ptr<common::Application::Renderer>&, const std::string&);
	void Start();
}