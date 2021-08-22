#pragma once
#include <Application.Engine.h>
#include <map>
#include <memory>
#include <SDL.h>
#include <string>
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

