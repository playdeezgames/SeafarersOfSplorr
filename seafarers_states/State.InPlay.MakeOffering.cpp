#include <Common.Data.h>
#include <Common.Utility.Table.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Items.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.MakeOffering.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MAKE_OFFERING;

	static void OnLeave()
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static std::map<game::Item, size_t> items;

	static void UpdateItems()
	{
		items = game::character::Items::All(GetPlayerCharacterId());
	}

	static void RefreshItems()
	{
		int index = 1;
		for (auto& item : items)
		{
			Terminal::WriteLine("{}) {}", index++, game::Items::GetName(item.first));
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Make Offering:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshItems();
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateItems();
		Refresh();
	}

	static void OnOfferingResult(game::OfferingResult result)
	{
		Terminal::WriteLine();
		switch (result)
		{
		case game::OfferingResult::BLESSING:
			Terminal::WriteLine("Blessing Received!");
			Terminal::WriteLine("The demigod is sufficiently pleased with you and decides to bless you!");
			break;
		case game::OfferingResult::CURSE:
			Terminal::WriteLine("Curse Received!");
			Terminal::WriteLine("The demigod is sufficiently displeased with you and decides to curse you!");
			break;
		case game::OfferingResult::COOLING_DOWN:
			Terminal::WriteLine("You have given enough for now...");
			Terminal::WriteLine("...try again later.");
			break;
		case game::OfferingResult::SUCCESS:
			Terminal::WriteLine("Offering Accepted!");
			Terminal::WriteLine("Yer offering has been accepted.");
			break;
		}
	}

	static void OfferItem(int hiliteRow)
	{
		auto item = common::utility::Table::GetNthKey(items, hiliteRow).value();
		auto islandId = GetPlayerCharacterIslandId().value();
		auto island = game::Islands::Read(islandId).value();
		game::character::Items::Remove(GetPlayerCharacterId(), item, 1);
		OnOfferingResult(game::Demigods::MakeOffering(GetPlayerCharacterId(), island.patronDemigodId, item));
		OnLeave();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave }
	};

	static void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < items.size())
		{
			OfferItem(index);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void MakeOffering::Start()
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
