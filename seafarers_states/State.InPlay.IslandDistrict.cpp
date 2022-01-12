#include <algorithm>
#include "State.InPlay.Globals.h"
#include <Game.Session.h>
#include "State.InPlay.IslandDistrict.h"
#include "State.ScratchPad.IslandDistrict.h"
#include "State.ScratchPad.IslandFeature.h"
namespace state::in_play
{
	static constexpr ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_DISTRICT;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto district =
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetIsland()
			.GetDistricts()
			.GetDistrict(scratch_pad::IslandDistrict::GetDistrict());
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(district.GetName());
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static ::UIState GetUIStateForFeatureType(const game::island::FeatureType& featureType)
	{
		return
			::UIState::IN_PLAY_DELIVERY_SERVICE;//TODO: when there is more than one feature type, fix this!
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
			game::Session()
			.GetPlayer()
			.GetCharacter()
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
		MenuAction defaultAction = { "Leave district", application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}