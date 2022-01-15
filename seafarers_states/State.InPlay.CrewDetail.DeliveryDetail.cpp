#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.DeliveryDetail.h"
namespace state::in_play::crew_detail
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Delivery Detail:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
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