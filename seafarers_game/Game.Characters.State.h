#pragma once
namespace game::characters
{
	enum class State
	{
		AT_SEA,
		DOCK,
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
		TAVERN
	};
}
