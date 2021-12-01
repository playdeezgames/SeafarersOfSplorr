#include <format>
#include <Game.Character.h>
#include <Game.Character.Statistics.h>
#include <Game.Islands.DarkAlley.h>
#include "State.InPlay.DarkAlley.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DARK_ALLEY;

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static double GetMinimumWager()
	{
		return GetPlayerCharacterDarkAlleyMinimumWager().value();
	}

	static const auto ReadMoney = game::character::statistics::Money::Current;

	static void OnGamble()
	{
		if (ReadMoney(GetPlayerCharacterId()).value() >= GetMinimumWager())
		{
			DoPlayerCharacterAction(game::character::Action::START_GAMBLING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		Terminal::WriteLine("Come Back When Yer Serious!");
		Terminal::WriteLine("You don't have enough money!");
		Terminal::WriteLine("Minimum bet: {:.4f}", GetMinimumWager());
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dark Alley:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Gamble");
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}
	
	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnGamble},
		{ "0", OnLeave}
	};

	void DarkAlley::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}
