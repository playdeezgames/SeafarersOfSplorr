#include <Game.Session.h>
#include <Game.Session.Characters.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.InPlay.CrewDetail.InventoryDetail.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedItemType.h"
#include <Game.Session.Character.ItemTypes.h>
#include <Game.Session.Item.Type.h>
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
			game::session::character::ItemTypes(scratch_pad::CrewDetail::GetCharacterId())
			.GetItemType(scratch_pad::SelectedItemType::GetItemTypeId());
		Terminal::WriteLine("Item Type: {}", game::session::item::Type(itemType.GetItemType()).GetName());
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(
					currentState,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					currentState,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}