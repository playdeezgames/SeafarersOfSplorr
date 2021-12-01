#include <Common.Data.h>
#include <Game.Character.Items.h>
#include <Game.Character.Ship.h>
#include <Game.Character.Statistics.h>
#include <Game.Items.h>
#include "State.InPlay.Cargo.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CARGO;

	static auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT);

	static std::map<game::Item, size_t> manifest;

	static void UpdateManifest()
	{
		manifest = game::character::Items::All(GetPlayerCharacterId());
	}

	static double GetTonnage()
	{
		return game::character::Items::TotalTonnage(GetPlayerCharacterId());
	}

	static double GetAvailableTonnage()
	{
		return game::character::Ship::AvailableTonnage(GetPlayerCharacterId()).value();
	}

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Cargo:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine(
				"Tonnage: {:.3f} ({:d}%)",
				GetTonnage(),
				(int)(100.0 * GetTonnage() / GetAvailableTonnage()));
		Terminal::WriteLine(
			"Money: {:.3f}",
			GetPlayerCharacterMoney());
		Terminal::SetForeground(game::Colors::BROWN);
		Terminal::WriteLine("Manifest:");
		Terminal::SetForeground(game::Colors::YELLOW);
		size_t index = 1;
		for (auto entry : manifest)
		{
			Terminal::WriteLine("{}) {} (x{})", index++, game::Items::GetName(entry.first), entry.second);
		}
		Terminal::WriteLine("0) Never mind");
		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateManifest();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	static void OnHandleOther(const std::string& input)
	{
		auto index = common::Data::ToInt(input)-1;
		if (index < manifest.size())
		{
			Terminal::ErrorMessage("TODO: whatever one does with cargo...");
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
		}
		Refresh();
	}

	void Cargo::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnHandleOther));
	}
}