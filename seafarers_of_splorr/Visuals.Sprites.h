#pragma once
#include <string>
#include <SDL.h>
#include "Common.XY.h"
#include <optional>
#include <memory>
#include <tuple>
#include "Common.Application.h"
namespace visuals::Sprites
{
	void Draw(const std::string&, const std::shared_ptr<common::Application::Renderer>&, const ::common::XY<int>&, const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>&, double angle=0.0);
	std::optional<int> GetWidth(const std::string&);
}