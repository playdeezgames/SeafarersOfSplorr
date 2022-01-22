#include "State.InPlay.CrewDetail.DeliveryDetail.h"
#include "State.InPlay.CrewDetail.ConfirmAbandonDelivery.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedDelivery.h"
#include <Game.Session.Characters.h>
#include <Game.Session.Islands.h>
namespace state::in_play::crew_detail
{
	std::optional<int> DeliveryDetail::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Delivery Detail:");
		Terminal::SetForeground(game::Colors::GRAY);

		using islands = game::session::Islands;
		auto delivery = game::session::character::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto toIsland = islands::GetIsland(delivery.GetToIslandId());
		auto location = 
			game::session::Characters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId())
			.GetBerth()
			.GetShip()
			.GetLocation();
		Terminal::WriteLine("Destination: {}", toIsland.GetName());
		Terminal::WriteLine("Distance: {:.2f}", toIsland.DistanceFrom(location));
		Terminal::WriteLine(
			"Reward: {} x {}", 
			delivery.GetRewardItemType().GetName(),
			delivery.GetRewardQuantity());

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Abandon Delivery",application::UIState::DoGoTo(ConfirmAbandonDelivery::GetStateId) });
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(Deliveries::GetStateId) };
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
		Registrar::Register(stateId, [](int currentState) 
			{
				::application::OnEnter::AddHandler(
					currentState,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					currentState,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}