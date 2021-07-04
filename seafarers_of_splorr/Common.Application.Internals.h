#pragma once
#include <SDL.h>
#include <memory>
namespace common::Application
{
	void DoTheThing(SDL_Renderer*);
	struct Renderer
	{
		std::shared_ptr<SDL_Renderer> renderer;
		Renderer(SDL_Renderer* r);
		void Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle = 0.0) const;
	};
}