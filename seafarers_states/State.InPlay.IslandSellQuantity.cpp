#include <Common.Data.h>
#include <Game.Islands.Markets.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandSellQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL_QUANTITY;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_NEXT)}
	};

	void IslandSellQuantity::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}