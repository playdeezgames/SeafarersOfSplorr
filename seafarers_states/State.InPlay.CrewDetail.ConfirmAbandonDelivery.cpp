#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.ConfirmAbandonDelivery.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::crew_detail
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_CONFIRM_ABANDON_DELIVERY;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_RED);
		Terminal::WriteLine("Are you sure you want to abandon this delivery?");
		Terminal::SetForeground(game::Colors::GRAY);
		auto islands = game::Session().GetWorld().GetIslands();
		auto delivery = game::session::character::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto toIsland = islands.GetIsland(delivery.GetToIslandId());
		auto fromIsland = islands.GetIsland(delivery.GetFromIslandId());
		Terminal::WriteLine("This will negatively impact your reputation on both {} and {}!", toIsland.GetName(), fromIsland.GetName());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void DoAbandonDelivery()
	{
		//TODO: abandon the delivery
		Refresh();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Abandon Delivery", DoAbandonDelivery});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ConfirmAbandonDelivery::Start()
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
