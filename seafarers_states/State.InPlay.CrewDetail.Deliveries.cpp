#include <algorithm>
#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.CrewDetail.Deliveries.h"
#include "State.InPlay.CrewDetail.DeliveryDetail.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.CrewDetail.h"
#include "State.ScratchPad.SelectedDelivery.h"
namespace state::in_play::crew_detail
{
	std::optional<int> Deliveries::stateId = std::nullopt;

	static void Refresh()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character =
			game::session::Characters()
			.GetCharacter(characterId);

		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Deliveries:");
		Terminal::SetForeground(game::Colors::GRAY);
		if (!character.GetDeliveries().HasAny())
		{
			Terminal::WriteLine("You currently have no deliveries.");
		}
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> GoToDeliveryDetail(int deliveryId)
	{
		return [=]()
		{
			scratch_pad::SelectedDelivery::SetDeliveryId(deliveryId);
			application::UIState::Write(DeliveryDetail::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		int characterId = scratch_pad::CrewDetail::GetCharacterId();
		auto character =
			game::session::Characters()
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
							"{} (Dist: {:.2f}) (Time: {}) (Reward: {} x {})",
							toIsland.GetName(), 
							toIsland.DistanceFrom(location),
							delivery.GetTimeLimit(),
							delivery.GetRewardItemType().GetName(),
							delivery.GetRewardQuantity()), 
						GoToDeliveryDetail(delivery.operator int()) 
					});
			});

		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(CrewDetail::GetStateId) };
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}