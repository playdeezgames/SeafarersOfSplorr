#include "State.InPlay.DeliveryService.DeliveryList.h"
#include "State.InPlay.DeliveryService.DeliveryDetail.h"
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.IslandFeature.h"
#include "State.ScratchPad.SelectedDelivery.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.Session.Islands.h>
#include <Game.Session.Character.h>
#include <Game.Session.Island.Feature.h>
namespace state::in_play::delivery_service
{
	std::optional<int> DeliveryList::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto feature =
			game::session::island::Feature(scratch_pad::IslandFeature::GetFeatureId());
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
			application::UIState::Write(DeliveryDetail::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		using islands =
			game::session::Islands;
		auto fromIslandId =
			game::session::Character(game::session::Player::GetCharacterId())
			.GetIslandId();
		auto deliveries =
			game::session::island::Feature(scratch_pad::IslandFeature::GetFeatureId())
			.GetDeliveryService()
			.GetDeliveries()
			.GetDeliveries();
		auto fromIsland = islands::GetIsland(fromIslandId);
		std::for_each(
			deliveries.begin(), 
			deliveries.end(), 
			[fromIsland](const auto& delivery)
			{
				auto toIsland = islands::GetIsland(delivery.GetToIslandId());
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
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(DeliveryService::GetStateId) };
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