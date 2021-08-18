#include "Common.Application.Renderer.h"
namespace common::Application
{
	Renderer::Renderer(SDL_Renderer* r)
	{
		this->renderer = std::shared_ptr<SDL_Renderer>(r, SDL_DestroyRenderer);
	}
	void Renderer::Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle) const
	{
		SDL_RenderCopyEx(renderer.get(), texture.get(), source, destination, angle, nullptr, SDL_FLIP_NONE);
	}
}