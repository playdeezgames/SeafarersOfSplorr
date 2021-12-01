#include <Game.Character.Plights.h>
#include <Game.Character.Statistics.h>
#include "State.InPlay.CharacterStatus.h"
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AVATAR_STATUS;

	static const auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static void RefreshStatistics()
	{
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Money: {:.4f}", game::character::Statistics::ReadMoney(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Reputation: {:.1f}", game::character::Statistics::GetReputation(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Brawling: {:.1f}", game::character::Statistics::GetBrawling(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Health: {:.0f}", game::character::Statistics::GetHealth(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Satiety: {:.0f}", game::character::Statistics::GetSatiety(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Turns: {}", game::character::Statistics::GetTurnsRemaining(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Dignity: {:.0f}", game::character::Statistics::GetDignity(CrewDetail::GetCharacterId()));
		Terminal::WriteLine("Poshness: {:.0f}", game::character::Statistics::GetPoshness(CrewDetail::GetCharacterId()));
	}

	static void RefreshPlights()
	{
		auto inflicted = game::character::Plights::InflictedWith(CrewDetail::GetCharacterId());
		if (!inflicted.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Plights:");
			for (auto& plight : inflicted)
			{
				Terminal::SetForeground((game::character::Plights::GetType(plight) == game::character::PlightType::CURSE) ? (game::Colors::RED) : (game::Colors::GREEN));
				Terminal::WriteLine("{}", game::character::Plights::GetName(plight));
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