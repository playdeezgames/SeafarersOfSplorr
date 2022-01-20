#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.InPlay.CrewDetail.InventoryDetail.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedItemType.h"
namespace state::in_play::crew_detail
{
	std::optional<int> Inventory::stateId = std::nullopt;

	static void Refresh()
	{
		auto character = 
			game::Session()
			.GetCharacters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId());
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Inventory:");
		Terminal::SetForeground(game::Colors::GRAY);
		if (!character.GetItemTypes().HasAny())
		{
			Terminal::WriteLine("(nothing)");
		}
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> GoToItemTypeDetail(int itemTypeId)
	{
		return [itemTypeId]()
		{
			scratch_pad::SelectedItemType::SetItemTypeId(itemTypeId);
			application::UIState::Write(InventoryDetail::GetStateId);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId());
		auto itemTypes = character.GetItemTypes().GetItemTypes();
		std::for_each(
			itemTypes.begin(),
			itemTypes.end(),
			[](const auto& itemType) 
			{
				Terminal::menu.AddAction(
					{
						std::format(
							"{} x {}",
							itemType.GetItemType().GetName(),
							itemType.GetQuantity()),
						GoToItemTypeDetail(itemType.GetItemType().operator int())
					});
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(CrewDetail::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Inventory::Start()
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