#include <Game.Characters.Plights.h>
#include <Game.Session.h>
#include "State.InPlay.CharacterStatus.h"
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHARACTER_STATUS;

	static const auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static void RefreshStatistics()
	{
		auto counters = game::Session().GetCharacters().GetCharacter(GetCrewDetailCharacterId()).GetCounters();
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Reputation: {:.1f}", counters.GetCounter(game::characters::Counter::REPUTATION).GetValue());
		Terminal::WriteLine("Brawling: {:.1f}", 1.0);
		Terminal::WriteLine("Turns: {}", counters.GetCounter(game::characters::Counter::TURNS_REMAINING).GetValue());
	}

	static void RefreshPlights()
	{
		int characterId = GetCrewDetailCharacterId();
		auto inflicted = game::characters::Plights::InflictedWith(characterId);
		if (!inflicted.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Plights:");
			for (auto& plight : inflicted)
			{
				auto characterPlight = game::Session().GetCharacters().GetCharacter(characterId).GetPlights().GetPlight(plight);
				Terminal::SetForeground((characterPlight.GetType() == game::characters::PlightType::CURSE) ? (game::Colors::RED) : (game::Colors::GREEN));
				Terminal::WriteLine("{}", characterPlight.GetName());
			}
			Terminal::SetForeground(game::Colors::GRAY);
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Statistics:");
		RefreshStatistics();
		RefreshPlights();

		Terminal::SetForeground(game::Colors::YELLOW);
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
		{ "0", OnLeave}
	};

	void CharacterStatus::Start()
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
