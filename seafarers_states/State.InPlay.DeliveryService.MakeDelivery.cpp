#include "State.InPlay.DeliveryService.MakeDelivery.h"
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.Globals.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.Islands.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Deliveries.h>
namespace state::in_play::delivery_service
{
	std::optional<int> MakeDelivery::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Make Delivery:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto islandId = character.GetIslandId();
		if (!game::session::character::Deliveries(character.ToId()).HasDeliveriesFor(islandId))
		{
			Terminal::WriteLine("You have no deliveries for this location.");
		}
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu();
	static std::function<void()> DoCompleteDelivery(int deliveryId)
	{
		return 
			[deliveryId]() 
			{
				game::session::character::Delivery(deliveryId).Complete();
				UpdateMenu();
				Refresh();
			};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto toIslandId = character.GetIslandId();
		auto deliveries = game::session::character::Deliveries(character.ToId()).GetDeliveriesFor(toIslandId);
		using islands = game::session::Islands;
		std::for_each(
			deliveries.begin(),
			deliveries.end(),
			[](const auto& delivery) 
			{
				auto island = islands::GetIsland(delivery.GetFromIslandId());
				Terminal::menu.AddAction({ island.GetName(), DoCompleteDelivery(delivery.operator int())});
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(DeliveryService::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void MakeDelivery::Start()
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