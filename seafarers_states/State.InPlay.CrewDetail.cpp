#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL;

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST);

	static void RefreshFlags(int characterId)
	{
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);
		auto flags = character.GetFlags().GetAll();
		if (!flags.empty())
		{
			bool first = true;
			for (auto flag : flags)
			{
				auto& name = flag.GetName();
				if (first)
				{
					Terminal::Write("Status: {}", name);
				}
				else
				{
					Terminal::Write(", {}", name);
				}
				first = false;
			}
			Terminal::WriteLine();
		}
	}

	static void RefreshHitPoints(int characterId)
	{
		auto hitPoints = 
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId)
			.GetHitpoints();
		Terminal::WriteLine(
			"HP: {}/{}", 
			hitPoints.GetCurrent(),
			hitPoints.GetMaximum());
	}

	static void Refresh()
	{
		int characterId = GetCrewDetailCharacterId();
		auto character = 
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", character.GetName());
		Terminal::WriteLine("From: {}", character.GetOriginIsland().GetName());
		Terminal::WriteLine("Tribe: {}", character.GetTribe().GetName());
		RefreshFlags(characterId);
		RefreshHitPoints(characterId);

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Characteristics");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_CHARACTERISTICS) },
		{ "0", OnLeave}
	};

	void CrewDetail::Start()
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
				Terminal::INVALID_INPUT,
				Refresh));
	}
}