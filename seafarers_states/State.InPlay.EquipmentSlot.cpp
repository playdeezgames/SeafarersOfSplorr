#include <Common.Data.h>
#include <Game.EquipSlots.h>
#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.EquipmentSlot.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT_SLOT;
	static std::string NOTHING = "(nothing)";

	static void RefreshItems()
	{
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		auto avatarName = game::Session().GetCharacters().GetCharacter(GetCrewDetailCharacterId()).GetName();
		auto equipSlotName = game::EquipSlots::GetName(GetEquipmentSlot());
		Terminal::WriteLine("Equipping {}'s {}:", avatarName, equipSlotName);
		Terminal::SetForeground(game::Colors::GRAY);
		std::string itemName = NOTHING;
		Terminal::WriteLine("Currently Equipped: {}", itemName);
		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshItems();
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
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)}
	};

	static const void OnOtherInput(const std::string& line)
	{
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	void EquipmentSlot::Start()
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