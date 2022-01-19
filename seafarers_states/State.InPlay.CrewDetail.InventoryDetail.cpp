#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.InventoryDetail.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedItemType.h"
namespace state::in_play::crew_detail
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_INVENTORY_DETAIL;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Inventory Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto itemType =
			game::Session()
			.GetCharacters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId())
			.GetItemTypes().GetItemType(scratch_pad::SelectedItemType::GetItemTypeId());
		Terminal::WriteLine("Item Type: {}", itemType.GetItemType().GetName());
		Terminal::WriteLine("Quantity: {}", itemType.GetQuantity());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_INVENTORY) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void InventoryDetail::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE,
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE,
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}