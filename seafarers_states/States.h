#pragma once
namespace state
{ 
	struct About
	{
		static void Start(); 
	};
	struct ConfirmAbandon 
	{ 
		static void Start(); 
	};
	struct ConfirmOverwrite 
	{ 
		static void Start(); 
	};
	struct ConfirmQuit
	{
		static void Start();
	};
	struct LeavePlay 
	{ 
		static void Start(); 
	};
	struct LoadGame 
	{ 
		static void Start(); 
	};
	struct MainMenu
	{
		static void Start();
	};
	struct Options 
	{ 
		static void Start(); 
	};
	struct SaveGame
	{
		static void Start();
	};
	struct Splash 
	{ 
		static void Start(); 
	};
	struct StartGame
	{ 
		static void Start(); 
	};
	struct Statistics 
	{ 
		static void Start(); 
	};
	struct Tip
	{
		static void Start();
	};
	namespace in_play
	{
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
		struct ConfirmAbandonJob 
		{ 
			static void Start(); 
		};
		struct ConfirmReplaceJob 
		{ 
			static void Start(); 
		};
		struct Docked 
		{ 
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
		struct Win
		{
			static void Start();
		};
		struct WorldMap
		{
			static void Start();
		};
	}
}
namespace state::in_play::AtSea { void Start(); }
namespace state::in_play::CurrentJob { void Start(); }
namespace state::in_play::Shipyard { void Start(); }
namespace state::in_play::DarkAlleyEntrance { void Start(); }
namespace state::in_play::DarkAlley { void Start(); }
namespace state::in_play::Confirm { void Start(); }
namespace state::in_play::GambleStart { void Start(); }
namespace state::in_play::GambleFinish { void Start(); }
namespace state::in_play::Temple { void Start(); }

