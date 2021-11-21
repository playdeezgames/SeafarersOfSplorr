#pragma once
namespace game::ship
{
	enum class State
	{
		AT_SEA,
		DOCKED,
		CAREENED_TO_PORT,
		CAREENED_TO_STARBOARD
	};
}