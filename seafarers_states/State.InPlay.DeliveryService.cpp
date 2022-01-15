#include <Game.Session.h>
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.IslandFeature.h"
namespace state::in_play
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DELIVERY_SERVICE;

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
		Terminal::WriteLine(feature.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({"Job List", application::UIState::GoTo(::UIState::IN_PLAY_DELIVERY_SERVICE_JOB_LIST) });
		//TODO: make a delivery when possible
		MenuAction defaultAction = { "Leave", application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_DISTRICT) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DeliveryService::Start()
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