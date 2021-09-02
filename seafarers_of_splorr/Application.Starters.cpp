#include <Application.Keyboard.h>
#include <Application.Options.h>
#include <Application.UIState.h>
#include <Audio.Mux.h>
#include <Audio.Sfx.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include <functional>
#include <Game.h>
#include <Game.Achievements.h>
#include <States.h>
#include <Sublayouts.h>
#include <UIState.h>
#include <vector>
#include <Visuals.Layouts.h>
#include <Visuals.Colors.h>
#include <Visuals.Fonts.h>
#include <Visuals.Sprites.h>
#include <Visuals.Textures.h>
#include <Visuals.WorldMap.h>
#include <Visuals.Navigator.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Messages.h>
namespace application
{
	std::vector<std::function<void()>> starters =
	{
		application::UIState::DoSetFinalState(::UIState::QUIT),
		game::Achievements::Start,
		data::json::Stores::DoSetStoreFile(data::json::Store::COLORS, "config/graphics/colors.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SOUND_EFFECTS, "config/audio/sfx.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::MUSIC_THEMES, "config/audio/mux.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TEXTURES, "config/graphics/textures.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SPRITES, "config/graphics/sprites.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::FONTS, "config/graphics/fonts.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::LAYOUTS, "config/ui/layouts.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::KEYS, "config/keyboard.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::OPTIONS, "config/options.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TIPS, "config/tips.json",std::nullopt),
		visuals::Colors::DoSetStore(data::json::Store::COLORS),
		visuals::Textures::DoSetStore(data::json::Store::TEXTURES),
		visuals::Sprites::DoSetStore(data::json::Store::SPRITES),
		visuals::Fonts::DoSetStore(data::json::Store::FONTS),
		visuals::Layouts::DoSetStore(data::json::Store::LAYOUTS),
		visuals::Layouts::DoRegisterType("WorldMap", visuals::WorldMap::Internalize),
		visuals::Layouts::DoRegisterType("Navigator", visuals::Navigator::Internalize),
		visuals::Messages::Reset,
		application::Keyboard::DoSetStore(data::json::Store::KEYS),
		Options::DoSetStore(data::json::Store::OPTIONS),
		visuals::Confirmations::Reset,
		data::json::Stores::Start,
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::IN_MEMORY, ":memory:"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::AUTOSAVE, "autosave.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_1, "slot1.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_2, "slot2.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_3, "slot3.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_4, "slot4.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_5, "slot5.db"),
		audio::Mux::DoSetStore(data::json::Store::MUSIC_THEMES),
		audio::Sfx::DoSetStore(data::json::Store::SOUND_EFFECTS),
		state::Splash::Start,
		state::Tip::Start,
		state::MainMenu::Start,
		state::About::Start,
		state::ConfirmQuit::Start,
		state::Options::Start,
		state::Start::Start,
		state::LeavePlay::Start,
		game::DoAddResetter(visuals::Confirmations::Reset),
		game::DoAddResetter(visuals::Messages::Reset),
		game::Start,
		state::Statistics::Start,
		state::LoadGame::Start,
		state::SaveGame::Start,
		state::ConfirmAbandon::Start,
		state::ConfirmOverwrite::Start,
		state::in_play::AtSea::Start,
		state::in_play::ChangeSpeed::Start,
		state::in_play::Docked::Start,
		state::in_play::Win::Start,
		state::in_play::Lose::Start,
		state::in_play::WorldMap::Start,
		state::in_play::IslandJobs::Start,
		state::in_play::ConfirmReplaceJob::Start,
		state::in_play::ConfirmAbandonJob::Start,
		state::in_play::CurrentJob::Start,
		state::in_play::IslandTrade::Start,
		state::in_play::IslandBuy::Start,
		state::in_play::IslandSell::Start,
		state::in_play::Cargo::Start,
		state::in_play::ShipStatus::Start,
		state::in_play::Next::Start,
		state::in_play::Shipyard::Start,
		state::in_play::DarkAlleyEntrance::Start,
		state::in_play::DarkAlley::Start,
		state::in_play::Message::Start,
		state::in_play::Confirm::Start,
		state::in_play::GambleStart::Start,
		state::in_play::GambleFinish::Start,
		state::in_play::AtSeaCareenSelect::Start,
		state::in_play::CareenedToPort::Start,
		state::in_play::CareenedToStarboard::Start,
		state::in_play::Temple::Start,
		state::in_play::MakeOffering::Start,
		state::in_play::AvatarStatus::Start,
		sublayout::UIHamburger::Start,
		Options::Initialize,
		visuals::Layouts::Start
	};

	const std::vector<std::function<void()>>& Engine::GetStarters()
	{
		return starters;
	}
}