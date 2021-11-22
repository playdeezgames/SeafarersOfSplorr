#pragma once
namespace game::character
{
	enum class State
	{
		AT_SEA, //TODO: make into ship state
		DOCK, //TODO: make into ship state
		JOB_BOARD,
		MARKET,
		MARKET_BUY,
		MARKET_SELL,
		SHIPYARD,
		DARK_ALLEY_ENTRANCE,
		DARK_ALLEY,
		GAMBLE_START,
		CAREENED_TO_PORT,//TODO: make into ship state
		CAREENED_TO_STARBOARD,//TODO: make into ship state
		TEMPLE,
		FISHING
		//TODO: ABOARD_SHIP (replaces AT_SEA)
		//TODO: ON_DOCK (replaced DOCK)
	};
}
