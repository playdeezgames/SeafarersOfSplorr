#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Character.Quest.h>
#include <Game.Character.Plights.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Avatar.StatisticFormats.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.AvatarStatus.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AVATAR_STATUS;

	static const auto OnLeave = ::application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static void RefreshStatistics()
	{
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Money: {:.4f}", game::avatar::Statistics::ReadMoney(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Reputation: {:.1f}", game::avatar::Statistics::GetReputation(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Brawling: {:.1f}", game::avatar::Statistics::GetBrawling(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Health: {:.0f}", game::avatar::Statistics::GetHealth(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Satiety: {:.0f}", game::avatar::Statistics::GetSatiety(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Turns: {}", game::avatar::Statistics::GetTurnsRemaining(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Dignity: {:.0f}", game::avatar::Statistics::GetDignity(CrewDetail::GetAvatarId()));
		Terminal::WriteLine("Poshness: {:.0f}", game::avatar::Statistics::GetPoshness(CrewDetail::GetAvatarId()));
	}

	static void RefreshPlights()
	{
		auto inflicted = game::character::Plights::InflictedWith();
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
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	void AvatarStatus::Start()
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
