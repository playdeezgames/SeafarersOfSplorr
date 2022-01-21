#include "State.InPlay.CrewDetail.ConfirmAbandonDelivery.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.SelectedDelivery.h"
#include <Game.Session.Character.Delivery.h>
#include <Game.Session.World.Islands.h>
namespace state::in_play::crew_detail
{
	std::optional<int> ConfirmAbandonDelivery::stateId;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_RED);
		Terminal::WriteLine("Are you sure you want to abandon this delivery?");
		Terminal::SetForeground(game::Colors::GRAY);
		auto islands = game::session::world::Islands();
		auto delivery = game::session::character::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto toIsland = islands.GetIsland(delivery.GetToIslandId());
		auto fromIsland = islands.GetIsland(delivery.GetFromIslandId());
		Terminal::WriteLine("This will negatively impact your reputation on both {} and {}!", toIsland.GetName(), fromIsland.GetName());
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void DoAbandonDelivery()
	{
		game::session::character::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId()).Abandon();
		application::UIState::Write(crew_detail::Deliveries::GetStateId());
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Abandon Delivery", DoAbandonDelivery});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(crew_detail::Deliveries::GetStateId) };
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
		Registrar::Register(stateId, [](int state) 
			{
				::application::OnEnter::AddHandler(
					state,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					state,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					state,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}
