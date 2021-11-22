#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.h>
#include <Common.Utility.Navigator.h>
#include <Common.Utility.Table.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include <Game.Player.h>
#include "State.InPlay.MakeOffering.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Messages.h>
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
		items = game::character::Items::All(game::Player::GetAvatarId());
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
		auto location = game::character::Docked::ReadLocation().value();
		auto island = game::Islands::Read(location).value();
		game::character::Items::Remove(game::Player::GetAvatarId(), item, 1);
		OnOfferingResult(game::Demigods::MakeOffering(island.patronDemigod, item));
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
