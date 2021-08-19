#pragma once
#include <SDL.h>
#include <map>
#include <string>
#include <memory>
#include "Application.Engine.h"
namespace visuals::Textures
{
	void SetStore(int);
	template<typename TStore>
	std::function<void()> DoSetStore(const TStore& store)
	{
		return [store]()
		{
			SetStore((int)store);
		};
	}
	const std::shared_ptr<SDL_Texture>& Read(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
}

