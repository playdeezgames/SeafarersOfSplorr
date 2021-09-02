#pragma once
namespace state
{ 
	struct About
	{
		static void Start(); 
	};
	struct ConfirmQuit
	{
		static void Start();
	};
	namespace in_play
	{
		struct AtSeaCareenSelect
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
namespace state::MainMenu { void Start(); }
namespace state::Options { void Start(); }
namespace state::Splash { void Start(); }
namespace state::Start { void Start(); }
namespace state::LeavePlay { void Start(); }
namespace state::LoadGame { void Start(); }
namespace state::SaveGame { void Start(); }
namespace state::ConfirmAbandon { void Start(); }
namespace state::ConfirmOverwrite { void Start(); }
namespace state::Tip { void Start(); }
namespace state::Statistics { void Start(); }
namespace state::in_play::AtSea { void Start(); }
namespace state::in_play::Docked { void Start(); }
namespace state::in_play::Lose { void Start(); }
namespace state::in_play::IslandJobs { void Start(); }
namespace state::in_play::ConfirmReplaceJob { void Start(); }
namespace state::in_play::ConfirmAbandonJob { void Start(); }
namespace state::in_play::CurrentJob { void Start(); }
namespace state::in_play::IslandTrade { void Start(); }
namespace state::in_play::IslandBuy { void Start(); }
namespace state::in_play::IslandSell { void Start(); }
namespace state::in_play::Shipyard { void Start(); }
namespace state::in_play::DarkAlleyEntrance { void Start(); }
namespace state::in_play::DarkAlley { void Start(); }
namespace state::in_play::Confirm { void Start(); }
namespace state::in_play::GambleStart { void Start(); }
namespace state::in_play::GambleFinish { void Start(); }
namespace state::in_play::Temple { void Start(); }
namespace state::in_play::MakeOffering { void Start(); }
