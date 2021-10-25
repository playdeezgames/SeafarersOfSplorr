#pragma once
namespace state::in_play
{
	struct AtSea
	{
		static void DoAutomoveTimer(const unsigned int&);
		static void ToggleAutoMove();
		static bool IsAutoMoveEngaged();
		static void UpdateAutoMoveState();
		static void Start();
	};
	struct AtSeaOverview
	{
		static void Start();
	};
	struct AtSeaCareenSelect
	{
		static void Start();
	};
	struct AvatarStatus
	{
		static void Start();
	};
	struct Cargo
	{
		static void Start();
	};
	struct CareenedToPort
	{
		static void Start();
	};
	struct CareenedToStarboard
	{
		static void Start();
	};
	struct ChangeSpeed
	{
		static void Start();
	};
	struct Confirm
	{
		static void Start();
	};
	struct ConfirmAbandonJob
	{
		static void Start();
	};
	struct ConfirmReplaceJob
	{
		static void Start();
	};
	struct CrewDetail
	{
		static void SetAvatarId(int);
		static void Start();
	};
	struct CrewList
	{
		static void Start();
	};
	struct CurrentJob
	{
		static void Start();
	};
	struct DarkAlley
	{
		static void Start();
	};
	struct DarkAlleyEntrance
	{
		static void Start();
	};
	struct Docked
	{
		static void Start();
	};
	struct Equipment
	{
		static void Start();
	};
	struct Fishing
	{
		static void Start();
	};
	struct GambleFinish
	{
		static void Start();
	};
	struct GambleStart
	{
		static double GetCurrentWager();//TODO: is this the best place for this?
		static void Start();
	};
	struct IslandBuy
	{
		static void Start();
	};
	struct IslandJobs
	{
		static void Start();
	};
	struct IslandSell
	{
		static void Start();
	};
	struct IslandTrade
	{
		static void Start();
	};
	struct Lose
	{
		static void Start();
	};
	struct MakeOffering
	{
		static void Start();
	};
	struct Message
	{
		static void Start();
	};
	struct Next
	{
		static void Start();
	};
	struct ShipStatus
	{
		static void Start();
	};
	struct Shipyard
	{
		static void Start();
	};
	struct Temple
	{
		static void Start();
	};
	struct Win
	{
		static void Start();
	};
	struct WorldMap
	{
		static void Start();
	};
}


