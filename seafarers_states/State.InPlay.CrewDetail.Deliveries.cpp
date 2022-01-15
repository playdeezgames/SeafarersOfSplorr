#include <algorithm>
#include <Game.Session.h>
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedDelivery.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
namespace state::in_play::crew_detail
{
	static constexpr auto CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_DELIVERIES;

	static void Refresh()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);

		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Deliveries:");
		//TODO: if there are no deliveries, indicate that here
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> GoToDeliveryDetail(int deliveryId)
	{
		return [=]()
		{
			scratch_pad::SelectedDelivery::SetDeliveryId(deliveryId);
			application::UIState::Write(::UIState::IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL);
		};
	}

	static void UpdateMenu()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);
		auto location =
			character
			.GetBerth()
			.GetShip()
			.GetLocation();
		auto deliveries =
			character
			.GetDeliveries()
			.GetDeliveries();
		auto islands =
			game::Session()
			.GetWorld()
			.GetIslands();

		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);

		std::for_each(
			deliveries.begin(),
			deliveries.end(),
			[=](const auto& delivery)
			{
				auto toIsland = islands.GetIsland(delivery.GetToIslandId());
				Terminal::menu.AddAction({ 
						std::format(
							"{} ({:.2f})", 
							toIsland.GetName(), 
							toIsland.DistanceFrom(location)), 
						GoToDeliveryDetail(delivery.operator int()) 
					});
			});

		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Deliveries::Start()
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