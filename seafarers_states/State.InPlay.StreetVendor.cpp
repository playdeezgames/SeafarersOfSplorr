#include <Game.Session.h>
#include <Game.Session.Player.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandDistrict.h"
#include "State.InPlay.StreetVendor.h"
#include "State.ScratchPad.IslandFeature.h"
namespace state::in_play
{
	std::optional<int> StreetVendor::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		auto feature =
			game::session::Player()
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
		MenuAction defaultAction = { "Leave", application::UIState::DoGoTo(IslandDistrict::GetStateId)};
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void StreetVendor::Start()
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