#pragma once
#include <memory>
#include <SDL.h>
#include "Common.XY.h"
#include <string>
#include "Common.Application.h"
#include <optional>
#include "Visuals.HorizontalAlignment.h"
namespace visuals::Fonts
{
	void WriteText(const std::string&, const std::shared_ptr<common::Application::Renderer>&, const common::XY<int>&, const std::string& text, const std::string& color, const HorizontalAlignment& alignment);
	std::optional<std::string> GetGlyphSpriteName(const std::string&, char);
}
