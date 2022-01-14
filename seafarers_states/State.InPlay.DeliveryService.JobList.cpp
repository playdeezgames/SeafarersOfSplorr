#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.DeliveryService.JobList.h"
#include "State.ScratchPad.IslandFeature.h"
namespace state::in_play::delivery_service
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DELIVERY_SERVICE_JOB_LIST;

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
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
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
			[](const auto& delivery) 
			{
				//TODO: add delivery to list
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

	void JobList::Start()
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