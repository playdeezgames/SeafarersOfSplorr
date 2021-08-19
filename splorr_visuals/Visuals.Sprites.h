#pragma once
#include <string>
#include <SDL.h>
#include "Common.XY.h"
#include <optional>
#include <memory>
#include <tuple>
#include "Application.Engine.h"
namespace visuals::Sprites
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
	void Draw(const std::string&, const std::shared_ptr<application::Engine::Renderer>&, const ::common::XY<int>&, const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>&, double angle=0.0);
	std::optional<int> GetWidth(const std::string&);
}