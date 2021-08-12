#pragma once
#include "Common.XY.h"
#include "Game.Avatar.DockedAction.h"
#include "Game.Avatar.DockedState.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::Avatar
{
	void Reset(const game::Difficulty&);

	enum class MoveResult
	{
		MOVED,
		CLAMPED
	};
	MoveResult Move();

	enum class DockResult
	{
		DOCKED,
		COMPLETED_QUEST,
		ALREADY_DOCKED
	};
	std::optional<DockResult> Dock();
	std::optional<common::XY<double>> GetDockedLocation();
	bool DoDockedAction(const avatar::DockedAction&);
	std::optional<game::avatar::DockedState> GetDockedState();
}
