#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.DeliveryService.DeliveryDetail.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::delivery_service
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DELIVERY_SERVICE_JOB_DETAIL;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Job Detail:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto delivery = 
			game::session::island::delivery_service::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		auto fromIsland = game::session::Island(delivery.GetFromIslandId());
		auto toIsland = game::session::Island(delivery.GetToIslandId());
		auto distance = fromIsland.DistanceTo(toIsland);
		Terminal::WriteLine("Destination: {}", toIsland.GetName());
		Terminal::WriteLine("Distance: {:.2f}", distance);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void AcceptJob()
	{
		auto delivery =
			game::session::island::delivery_service::Delivery(scratch_pad::SelectedDelivery::GetDeliveryId());
		game::Session()
			.GetPlayer()
			.GetCharacter()
			.AcceptDelivery(delivery);
		application::UIState::Write(::UIState::IN_PLAY_DELIVERY_SERVICE_JOB_LIST);
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Accept", AcceptJob});
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_DELIVERY_SERVICE_JOB_LIST) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}