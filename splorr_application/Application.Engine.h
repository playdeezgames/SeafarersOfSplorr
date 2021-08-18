#pragma once
#include <memory>
#include <SDL.h>
namespace application::Engine
{
	struct Renderer
	{
		std::shared_ptr<SDL_Renderer> renderer;
		Renderer(SDL_Renderer* r);
		void Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle = 0.0) const;
	};
	void Render(const std::shared_ptr<Renderer>&);
	void SetFullscreen(bool);
	bool IsFullscreen();
}
