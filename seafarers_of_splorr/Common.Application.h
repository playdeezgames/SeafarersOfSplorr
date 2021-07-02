#pragma once
#include <SDL.h>
#include <memory>
namespace common::Application
{
	struct Renderer;
	void Render(const std::shared_ptr<Renderer>&);
	void SetFullscreen(bool);
	bool IsFullscreen();
}
