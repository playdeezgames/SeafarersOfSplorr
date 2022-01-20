#include <Game.Session.h>
#include <Game.Session.Characters.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.InPlay.CrewDetail.InventoryDetail.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedItemType.h"
namespace state::in_play::crew_detail
{
	std::optional<int> InventoryDetail::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Inventory Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto itemType =
			game::session::Characters()
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
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(Inventory::GetStateId) };
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}