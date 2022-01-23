#include <Game.Session.h>
#include <Game.Session.Player.h>
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.DeliveryService.DeliveryList.h"
#include "State.InPlay.DeliveryService.MakeDelivery.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.ScratchPad.IslandFeature.h"
#include <Game.Session.Character.h>
#include <Game.Session.Character.Deliveries.h>
#include <Game.Session.Island.Feature.h>
namespace state::in_play
{
	std::optional<int> DeliveryService::stateId = std::nullopt;

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

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Job List", application::UIState::DoGoTo(delivery_service::DeliveryList::GetStateId) });
		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto islandId = character.GetIslandId();
		if (game::session::character::Deliveries(character.ToId()).HasDeliveriesFor(islandId))
		{
			Terminal::menu.AddAction({"Make Delivery", application::UIState::DoGoTo(delivery_service::MakeDelivery::GetStateId) });
		}
		MenuAction defaultAction = { "Leave", application::UIState::DoGoTo(IslandDistrict::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DeliveryService::Start()
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