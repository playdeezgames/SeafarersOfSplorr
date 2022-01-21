#include "State.InPlay.DeliveryService.MakeDelivery.h"
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.Globals.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.World.Islands.h>
namespace state::in_play::delivery_service
{
	std::optional<int> MakeDelivery::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Make Delivery:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto character = game::session::Player().GetCharacter();
		auto island = character.GetIsland();
		if (!character.GetDeliveries().HasDeliveriesFor(island.operator int()))
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
		auto character = game::session::Player().GetCharacter();
		auto toIsland = character.GetIsland();
		auto deliveries = character.GetDeliveries().GetDeliveriesFor(toIsland.operator int());
		auto islands = game::session::world::Islands();
		std::for_each(
			deliveries.begin(),
			deliveries.end(),
			[islands](const auto& delivery) 
			{
				auto island = islands.GetIsland(delivery.GetFromIslandId());
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