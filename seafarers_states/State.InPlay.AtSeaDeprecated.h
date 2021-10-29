#pragma once
namespace state::in_play
{
	struct AtSeaDeprecated
	{
		static void DoAutomoveTimer(const unsigned int&);
		static void ToggleAutoMove();
		static bool IsAutoMoveEngaged();
		static void UpdateAutoMoveState();
		static void Start();
	};
}