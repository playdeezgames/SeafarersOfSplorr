#include <Common.Data.h>
#include <Common.Utility.Table.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Items.h>
#include <Game.Character.Statistics.h>
#include <Game.Islands.Items.h>
#include <Game.Items.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandSell.h"
#include "State.InPlay.IslandSellQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL;

	static void OnLeave()
	{
		game::character::Actions::DoAction(GetPlayerCharacterId(), game::character::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static std::map<game::Item, double> unitPrices;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetSalePrices(GetPlayerCharacterIslandId().value());
	}

	static void RefreshUnitPrices()
	{
		int index = 1;
		for (auto& unitPrice : unitPrices)
		{
			Terminal::WriteLine("{}) {} ({} @ {:.4f})",
				index++,
				game::Items::GetName(unitPrice.first),
				game::character::Items::Read(GetPlayerCharacterId(), unitPrice.first),
				unitPrice.second);
		}
	}

	static const std::string FORMAT_MONEY = "Money: {:.3f}";

	static void RefreshStatistics()
	{
		Terminal::WriteLine(FORMAT_MONEY, game::character::Statistics::ReadMoney(GetPlayerCharacterId()));
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Sell:");
		Terminal::SetForeground(game::Colors::GRAY);
		RefreshStatistics();
		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshUnitPrices();
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateUnitPrices();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < unitPrices.size())
		{
			auto key = common::utility::Table::GetNthKey(unitPrices, index).value();
			IslandSellQuantity::SetItem(common::utility::Table::GetNthKey(unitPrices, index).value());
			application::UIState::Write(::UIState::IN_PLAY_ISLAND_SELL_QUANTITY);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void IslandSell::Start()
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
