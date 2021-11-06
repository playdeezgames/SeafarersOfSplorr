#pragma once
namespace state::in_play
{
	struct GambleStartDeprecated
	{
		static double GetCurrentWager();//TODO: is this the best place for this?
		static void Start();
	};
}