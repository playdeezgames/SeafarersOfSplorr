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
		NORMAL,
		CLAMPED
	};
	MoveResult Move();

	bool Dock();
	std::optional<common::XY<double>> GetDockedLocation();
	bool Undock();
}
