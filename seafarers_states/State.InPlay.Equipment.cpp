#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Equipment.h>
#include <Game.Avatar.Items.h>
#include <Game.Colors.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Equipment.h"
#include "State.InPlay.EquipmentSlot.h"
#include "State.Terminal.h"
#include "UIState.h"
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

		auto item = game::character::Equipment::Read(CrewDetail::GetAvatarId(), equipSlot);
		std::string itemName = item.has_value() ? (game::Items::GetName(item.value())) : EMPTY;
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
		Terminal::WriteLine("Equipment for {}:", game::Character::GetName(CrewDetail::GetAvatarId()).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshEquipSlots();
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
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL)}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < equipSlots.size())
		{
			EquipmentSlot::SetSlot(equipSlots[index]);
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
