#include "Common.Application.Internals.h"
namespace common::Application
{
	void DoTheThing(SDL_Renderer* renderer)
	{
		SDL_DestroyRenderer(renderer);
	}
	Renderer::Renderer(SDL_Renderer* r)
	{
		this->renderer = std::shared_ptr<SDL_Renderer>(r, DoTheThing);
	}
	void Renderer::Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle) const
	{
		SDL_RenderCopyEx(renderer.get(), texture.get(), source, destination, angle, nullptr, SDL_FLIP_NONE);
	}
}