#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Game.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
namespace state::LoadGame
{
	const std::string LAYOUT_NAME = "State.LoadGame";
	const std::string MENU_ID = "LoadGame";
	const std::string MENU_ITEM_AUTOSAVE = "Autosave";
	const std::string MENU_ITEM_SLOT1 = "Slot1";
	const std::string MENU_ITEM_SLOT2 = "Slot2";
	const std::string MENU_ITEM_SLOT3 = "Slot3";
	const std::string MENU_ITEM_SLOT4 = "Slot4";
	const std::string MENU_ITEM_SLOT5 = "Slot5";
	const std::string AUTOSAVE_PRESENT = "(autosave)";
	const std::string NOT_PRESENT = "-";
	const std::string SLOT_PRESENT_1 = "Slot 1";
	const std::string SLOT_PRESENT_2 = "Slot 2";
	const std::string SLOT_PRESENT_3 = "Slot 3";
	const std::string SLOT_PRESENT_4 = "Slot 4";
	const std::string SLOT_PRESENT_5 = "Slot 5";

	enum class LoadGameItem
	{
		AUTOSAVE,
		SLOT_1,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5,
		BACK
	};

	static void GoBack()
	{
		::application::UIState::Write(::UIState::START_GAME);
	}

	static void LoadFromAutosave()
	{
		if (game::DoesAutosaveExist())
		{
			game::LoadFromAutosave();
			application::UIState::Write(::UIState::TIP);
		}
	}

	static std::function<void()> SlotLoader(int slot)
	{
		return [slot]()
		{
			if (game::DoesSlotExist(slot))
			{
				game::LoadFromSlot(slot);
				application::UIState::Write(::UIState::TIP);
			}
		};
	}

	const std::map<LoadGameItem, std::function<void()>> activators =
	{
		{ LoadGameItem::AUTOSAVE, LoadFromAutosave },
		{ LoadGameItem::SLOT_1, SlotLoader(1) },
		{ LoadGameItem::SLOT_2, SlotLoader(2) },
		{ LoadGameItem::SLOT_3, SlotLoader(3) },
		{ LoadGameItem::SLOT_4, SlotLoader(4) },
		{ LoadGameItem::SLOT_5, SlotLoader(5) },
		{ LoadGameItem::BACK, GoBack }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LoadGameItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, GoBack },
		{ ::Command::RED, GoBack },
		{ ::Command::GREEN, ActivateItem }
	};

	static void UpdateMenuItem(bool shown, const std::string& menuItem, const std::string& textWhenShown)
	{
		if (shown)
		{
			visuals::MenuItems::SetText(LAYOUT_NAME, menuItem, textWhenShown);
		}
		else
		{
			visuals::MenuItems::SetText(LAYOUT_NAME, menuItem, NOT_PRESENT);
		}
	}

	static void OnEnter()
	{
		UpdateMenuItem(game::DoesAutosaveExist(), MENU_ITEM_AUTOSAVE, AUTOSAVE_PRESENT);
		UpdateMenuItem(game::DoesSlotExist(1), MENU_ITEM_SLOT1, SLOT_PRESENT_1);
		UpdateMenuItem(game::DoesSlotExist(2), MENU_ITEM_SLOT2, SLOT_PRESENT_2);
		UpdateMenuItem(game::DoesSlotExist(3), MENU_ITEM_SLOT3, SLOT_PRESENT_3);
		UpdateMenuItem(game::DoesSlotExist(4), MENU_ITEM_SLOT4, SLOT_PRESENT_4);
		UpdateMenuItem(game::DoesSlotExist(5), MENU_ITEM_SLOT5, SLOT_PRESENT_5);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::LOAD_GAME, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::LOAD_GAME, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::LOAD_GAME, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::LOAD_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LOAD_GAME, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::LOAD_GAME, OnEnter);
	}
}