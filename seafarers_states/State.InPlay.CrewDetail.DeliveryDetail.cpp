#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.DeliveryDetail.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::crew_detail
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Delivery Detail:");
		Terminal::SetForeground(game::Colors::GRAY);

		auto islands = game::Session().GetWorld().GetIslands();
		auto delivery = game::session::character::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto toIsland = islands.GetIsland(delivery.GetToIslandId());
		auto location = 
			game::Session()
			.GetCharacters()
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
		Terminal::menu.AddAction({ "Abandon Delivery",application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_CONFIRM_ABANDON_DELIVERY) });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_DELIVERIES) };
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
		::application::OnEnter::AddHandler(
			CURRENT_STATE,
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE,
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}