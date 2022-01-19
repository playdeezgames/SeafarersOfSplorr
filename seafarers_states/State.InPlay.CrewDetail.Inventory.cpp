#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.Inventory.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedItemType.h"
namespace state::in_play::crew_detail
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_INVENTORY;

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
			application::UIState::Write(::UIState::IN_PLAY_CREW_DETAIL_INVENTORY_DETAIL);
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
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL) };
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