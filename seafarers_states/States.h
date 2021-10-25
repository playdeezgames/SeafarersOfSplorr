#pragma once
namespace state::in_play
{
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
	struct WorldMap
	{
		static void Start();
	};
}


