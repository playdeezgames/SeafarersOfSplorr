#include <Game.Session.h>
#include <Game.Session.Player.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.DeliveryService.DeliveryDetail.h"
#include "State.InPlay.DeliveryService.DeliveryList.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::delivery_service
{
	std::optional<int> DeliveryDetail::stateId = std::nullopt;

	using Delivery = game::session::Delivery;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Job Detail:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto delivery = 
			Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto fromIsland = game::session::Island(delivery.GetFromIslandId());
		auto toIsland = game::session::Island(delivery.GetToIslandId());
		auto distance = fromIsland.DistanceTo(toIsland);
		Terminal::WriteLine("Destination: {}", toIsland.GetName());
		Terminal::WriteLine("Distance: {:.2f}", distance);
		Terminal::WriteLine(
			"Reward: {} x {}", 
			delivery.GetRewardItemType().GetName(), 
			delivery.GetRewardQuantity());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void AcceptJob()
	{
		auto delivery =
			Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		game::session::Player()
			.GetCharacter()
			.AcceptDelivery(delivery);
		application::UIState::Write(DeliveryList::GetStateId());
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Accept", AcceptJob});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(DeliveryList::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DeliveryDetail::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}