#include <algorithm>
#include <format>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.DeliveryService.DeliveryList.h"
#include "State.ScratchPad.IslandFeature.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::delivery_service
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DELIVERY_SERVICE_DELIVERY_LIST;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto feature =
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetIsland()
			.GetFeature(scratch_pad::IslandFeature::GetFeatureId());
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("{} Job List:", feature.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		if (!feature.GetDeliveryService().HasDeliveries())
		{
			Terminal::WriteLine("There are no deliveries available.");
		}
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> DoSelectDelivery(int deliveryId)
	{
		return [deliveryId]() 
		{
			scratch_pad::SelectedDelivery::SetDeliveryId(deliveryId);
			application::UIState::Write(::UIState::IN_PLAY_DELIVERY_SERVICE_DELIVERY_DETAIL);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto islands =
			game::Session()
			.GetWorld()
			.GetIslands();
		auto fromIsland =
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetIsland();
		auto deliveries =
			fromIsland
			.GetFeature(scratch_pad::IslandFeature::GetFeatureId())
			.GetDeliveryService()
			.GetDeliveries()
			.GetDeliveries();
		std::for_each(
			deliveries.begin(), 
			deliveries.end(), 
			[islands, fromIsland](const auto& delivery) 
			{
				auto toIsland = islands.GetIsland(delivery.GetToIslandId());
				auto distance = fromIsland.DistanceTo(toIsland);
				auto timeLimit = delivery.GetTimeLimit();
				Terminal::menu.AddAction(
					{ 
						std::format(
							"Dist: {:.2f}, Time Limit: {}, Reward: {} x {}", 
							distance, 
							timeLimit,
							delivery.GetRewardItemType().GetName(),
							delivery.GetRewardQuantity()), 
						DoSelectDelivery(delivery.operator int())
					});
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

	void DeliveryList::Start()
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