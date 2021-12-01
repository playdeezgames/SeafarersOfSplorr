#include <Game.Character.Statistics.h>
#include <Game.Islands.DarkAlley.h>
#include "State.InPlay.GambleIntro.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_INTRO;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Gambling:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto ante = GetPlayerCharacterDarkAlleyAnte().value();
		auto minimum = GetPlayerCharacterDarkAlleyMinimumWager().value();
		Terminal::WriteLine("Yer money: {:.4f}", GetPlayerCharacterMoney().value());
		Terminal::WriteLine("Minimum bet: {:.4f}", minimum);
		Terminal::WriteLine("Ante: {:.4f}", ante);

		Terminal::SetForeground(game::Colors::YELLOW);
		if (GetPlayerCharacterMoney().value() >= minimum)
		{
			Terminal::WriteLine("1) Play a hand");
		}
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::character::Action::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void DealHand()
	{
		if (GetPlayerCharacterMoney().value() >= GetPlayerCharacterDarkAlleyMinimumWager().value())
		{
			ChangePlayerCharacterMoney(-GetPlayerCharacterDarkAlleyAnte().value());
			application::UIState::Write(::UIState::IN_PLAY_GAMBLE_PLAY);
		}
		else
		{
			Terminal::WriteLine(Terminal::INVALID_INPUT);
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", DealHand},
		{ "0", OnLeave}
	};

	void GambleIntro::Start()
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