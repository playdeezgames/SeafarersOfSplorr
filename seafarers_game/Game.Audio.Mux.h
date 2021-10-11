#pragma once
#include <functional>
#include "Game.Audio.Theme.h"
namespace game::audio//20211011
{
	struct Mux
	{
		static void Play(const Theme&);
		static std::function<void()> GoToTheme(const Theme&);
	};
}

