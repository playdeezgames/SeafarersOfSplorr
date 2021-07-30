#pragma once
#include "Common.XY.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::Avatar
{
	void Reset(const game::Difficulty&);

	double GetHeading();
	void SetHeading(double);

	double GetSpeed();
	void SetSpeed(double);

	common::XY<double> GetLocation();
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
	bool Undock();
}
