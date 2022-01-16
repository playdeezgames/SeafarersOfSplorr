#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.DeliveryService.MakeDelivery.h"
#include "State.InPlay.Globals.h"
namespace state::in_play::delivery_service
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_DELIVERY_SERVICE_MAKE_DELIVERY;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Make Delivery:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto character = game::Session().GetPlayer().GetCharacter();
		auto island = character.GetIsland();
		if (!character.GetDeliveries().HasDeliveriesFor(island.operator int()))
		{
			Terminal::WriteLine("You have no deliveries for this location.");
		}
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> DoCompleteDelivery(int deliveryId)
	{
		return 
			[deliveryId]() 
			{
				game::session::character::Delivery(deliveryId).Complete();
				Refresh();
			};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		auto character = game::Session().GetPlayer().GetCharacter();
		auto toIsland = character.GetIsland();
		auto deliveries = character.GetDeliveries().GetDeliveriesFor(toIsland.operator int());
		auto islands = game::Session().GetWorld().GetIslands();
		std::for_each(
			deliveries.begin(),
			deliveries.end(),
			[islands](const auto& delivery) 
			{
				auto island = islands.GetIsland(delivery.GetFromIslandId());
				Terminal::menu.AddAction({ island.GetName(), DoCompleteDelivery(delivery.operator int())});
			});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_DELIVERY_SERVICE) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}