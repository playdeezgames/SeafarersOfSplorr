#include <Game.Session.h>
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.DeliveryService.DeliveryList.h"
#include "State.InPlay.DeliveryService.MakeDelivery.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.ScratchPad.IslandFeature.h"
namespace state::in_play
{
	std::optional<int> DeliveryService::stateId = std::nullopt;

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
		Terminal::menu.AddAction({"Job List", application::UIState::DoGoTo(delivery_service::DeliveryList::GetStateId) });
		auto character = game::Session().GetPlayer().GetCharacter();
		auto island = character.GetIsland();
		if (character.GetDeliveries().HasDeliveriesFor(island.operator int()))
		{
			Terminal::menu.AddAction({"Make Delivery", application::UIState::DoGoTo(delivery_service::MakeDelivery::GetStateId) });
		}
		MenuAction defaultAction = { "Leave", application::UIState::DoGoTo(IslandDistrict::GetStateId) };
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}