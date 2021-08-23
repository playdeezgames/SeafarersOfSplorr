#pragma once
#include <Common.XY.h>
#include "Game.Avatar.DockedAction.h"
#include "Game.Avatar.State.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar::Docked
{
	void Reset(const game::Difficulty&);

	enum class DockResult
	{
		DOCKED,
		COMPLETED_QUEST,
		ALREADY_DOCKED
	};
	std::optional<DockResult> Dock();
	std::optional<common::XY<double>> GetDockedLocation();

	bool DoDockedAction(const avatar::DockedAction&);
	std::optional<game::avatar::State> GetDockedState();
}
