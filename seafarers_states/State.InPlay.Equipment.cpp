#include <Common.Data.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Equipment.h"
#include "State.InPlay.EquipmentSlot.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT;
	static const std::string EMPTY = "(empty)";

	static const std::vector<game::EquipSlot> equipSlots =
	{
		game::EquipSlot::LEGS,
		game::EquipSlot::PRISON_WALLET
	};

	static void RefreshEquipSlot(size_t index, const game::EquipSlot& equipSlot)
	{
		auto slotName = game::EquipSlots::GetName(equipSlot);

		std::string itemName = EMPTY;
		Terminal::WriteLine("{}) {} - {}", index, slotName, itemName);
	}

	static void RefreshEquipSlots()
	{
		size_t index = 1;
		for (auto& equipSlot : equipSlots)
		{
			RefreshEquipSlot(index, equipSlot);
			++index;
		}
	}


	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Equipment for {}:", game::Session().GetCharacters().GetCharacter(GetCrewDetailCharacterId()).GetName());

		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshEquipSlots();
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
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL)}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < equipSlots.size())
		{
			SetEquipmentSlot(equipSlots[index]);
			application::UIState::Write(::UIState::IN_PLAY_EQUIPMENT_SLOT);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void Equipment::Start()
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
				OnOtherInput));
	}
}
