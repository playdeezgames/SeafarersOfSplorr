#pragma once
#include "Game.Achievement.h"
#include <optional>
namespace game::Achievements
{
	void Add(const game::Achievement&);
	void Start();
	bool ShouldRestartApp();
}
