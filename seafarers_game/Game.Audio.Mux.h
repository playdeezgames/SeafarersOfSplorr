#pragma once
#include <functional>
#include "Game.Audio.Theme.h"
namespace game::audio::Mux
{
	void Play(const Theme&);
	std::function<void()> GoToTheme(const Theme&);
}

