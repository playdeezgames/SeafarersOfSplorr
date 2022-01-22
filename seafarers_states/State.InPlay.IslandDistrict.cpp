#include <algorithm>
#include "State.InPlay.Globals.h"
#include <Game.Session.Player.h>
#include "State.InPlay.Docked.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.InPlay.DeliveryService.h"
#include "State.InPlay.StreetVendor.h"
#include "State.ScratchPad.IslandDistrict.h"
#include "State.ScratchPad.IslandFeature.h"
#include <Game.Session.Character.h>
namespace state::in_play
{
	std::optional<int> IslandDistrict::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto district =
			game::session::Character(game::session::Player::GetCharacterId())
			.GetIsland()
			.GetDistricts()
			.GetDistrict(scratch_pad::IslandDistrict::GetDistrict());
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(district.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static int GetUIStateForFeatureType(const game::island::FeatureType& featureType)
	{
		return
			(featureType== game::island::FeatureType::DELIVERY_SERVICE) ? (DeliveryService::GetStateId()) :
			(featureType == game::island::FeatureType::STREET_VENDOR) ? (StreetVendor::GetStateId()) :
			(throw "no dice!");
	}

	static std::function<void()> GoToFeature(int featureId, const game::island::FeatureType& featureType)
	{
		auto uiState = GetUIStateForFeatureType(featureType);
		return [featureId, uiState]() 
		{
			scratch_pad::IslandFeature::SetFeatureId(featureId);
			application::UIState::Write(uiState);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto features =
			game::session::Character(game::session::Player::GetCharacterId())
			.GetIsland()
			.GetDistricts()
			.GetDistrict(scratch_pad::IslandDistrict::GetDistrict())
			.GetFeatures()
			.GetFeatures();
		std::for_each(
			features.begin(), 
			features.end(), 
			[](const game::session::island::Feature& feature) 
			{
				Terminal::menu.AddAction({ feature.GetName() , GoToFeature(feature.operator int(), feature.GetFeatureType()) });
			});
		MenuAction defaultAction = { "Leave district", application::UIState::DoGoTo(Docked::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void IslandDistrict::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}