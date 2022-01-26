#include "State.InPlay.StreetVendor.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.ScratchPad.IslandFeature.h"
#include <algorithm>
#include <format>
#include <Game.Colors.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.ItemType.h>
#include <Game.Session.Character.Messages.h>
#include <Game.Session.h>
#include <Game.Session.Island.Feature.h>
#include <Game.Session.Island.StreetVendor.MenuItems.h>
#include <Game.Session.Player.h>
#include <Game.Session.World.h>
#include <Game.Session.Item.Type.h>
namespace state::in_play
{
	std::optional<int> StreetVendor::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto feature =
			game::session::island::Feature(scratch_pad::IslandFeature::GetFeatureId());
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(feature.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> DoBuyMenuItem(int menuItemId)
	{
		return [menuItemId]() 
		{
			auto characterId = game::session::Player::GetCharacterId();
			auto menuItem = game::session::island::street_vendor::MenuItem(menuItemId);
			auto jools = game::session::character::ItemType(
				characterId,
				game::session::World::GetCurrencyItemSubtype().operator int());
			auto price = menuItem.GetPrice();
			if (jools.GetQuantity() >= price)
			{
				jools.RemoveQuantity(price);
				menuItem.Feed(characterId);
			}
			else
			{
				game::session::character::Messages(characterId).Add(game::Colors::RED, "You don't have enough jools.");
			}
			Refresh();
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto menuItems =
			game::session::island::street_vendor::MenuItems(
				scratch_pad::IslandFeature::GetFeatureId())
			.GetMenuItems();
		std::for_each(
			menuItems.begin(), 
			menuItems.end(), 
			[](const auto& menuItem) 
			{
				auto itemType = game::session::item::Type(menuItem.GetItemTypeId());
				Terminal::menu.AddAction({ 
					std::format("{} (Price:{})",itemType.GetName(), menuItem.GetPrice()), DoBuyMenuItem(menuItem.ToId()) });
			});
		MenuAction defaultAction = { "Leave", application::UIState::DoGoTo(IslandDistrict::GetStateId)};
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void StreetVendor::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}