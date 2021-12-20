#include <Game.Characters.Plights.h>
#include <Game.Characters.Statistics.h>
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
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Reputation: {:.1f}", game::characters::statistics::Reputation::Current(GetCrewDetailCharacterId()).value());
		Terminal::WriteLine("Brawling: {:.1f}", game::characters::statistics::Brawling::Current(GetCrewDetailCharacterId()).value());
		Terminal::WriteLine("Turns: {}", game::characters::statistics::Turns::Remaining(GetCrewDetailCharacterId()).value());
	}

	static void RefreshPlights()
	{
		auto inflicted = game::characters::Plights::InflictedWith(GetCrewDetailCharacterId());
		if (!inflicted.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Plights:");
			for (auto& plight : inflicted)
			{
				Terminal::SetForeground((game::characters::Plights::GetType(plight) == game::characters::PlightType::CURSE) ? (game::Colors::RED) : (game::Colors::GREEN));
				Terminal::WriteLine("{}", game::characters::Plights::GetName(plight));
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
