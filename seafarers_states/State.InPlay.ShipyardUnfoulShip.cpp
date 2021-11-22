#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Statistics.h>
#include <Game.Character.ShipStatistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Commodities.h>
#include <Game.Player.h>
#include <Game.World.h>
#include "State.InPlay.ShipyardUnfoulShip.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIPYARD_UNFOUL_SHIP;

	static double GetUnfoulingPrice()
	{
		return game::islands::Commodities::GetPurchasePrice(
			game::character::Docked::ReadLocation().value(),
			{
					{game::Commodity::LABOR, 
						game::World::GetUnfoulingLaborMultiplier() * 
						game::character::ShipStatistics::GetFouling()}
			});
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Unfoul Ship:");

		auto price = GetUnfoulingPrice();
		if (price > 0)
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("The price is {:.4f}.", price);
			if (game::character::Statistics::ReadMoney(game::Player::GetAvatarId()) >= price)
			{
				Terminal::SetForeground(game::Colors::YELLOW);
				Terminal::WriteLine("1) Clean hull");
			}
		}
		else
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Yer ship has no fouling.");
		}

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnCleanHull()
	{
		double price = GetUnfoulingPrice();
		if (price>0 && game::character::Statistics::ReadMoney(game::Player::GetAvatarId()) >= price)
		{
			game::character::Statistics::ChangeMoney(game::Player::GetAvatarId(), -price);
			game::character::ShipStatistics::CleanHull(game::Side::STARBOARD);
			game::character::ShipStatistics::CleanHull(game::Side::PORT);
			visuals::Messages::Write({ "You unfoul yer ship!" ,{}});
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnLeave()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnCleanHull },
		{ "0", OnLeave }
	};

	void ShipyardUnfoulShip::Start()
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