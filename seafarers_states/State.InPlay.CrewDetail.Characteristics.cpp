#include <Game.Session.h>
#include "State.InPlay.CrewDetail.Characteristics.h"
#include "State.InPlay.Globals.h"
#include "State.ScratchPad.CrewDetail.h"
namespace state::in_play::crew_detail
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_CHARACTERISTICS;

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static void Refresh()
	{
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(scratch_pad::CrewDetail::GetCharacterId());
		auto characteristics =
			character
			.GetCharacteristics()
			.GetAll();
		auto hitPoints =
			character
			.GetHitpoints();

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Characteristics:");

		Terminal::SetForeground(game::Colors::GRAY);
		for (auto characteristic : characteristics)
		{
			Terminal::WriteLine("{}: {}", characteristic.GetName(), characteristic.GetValue());
		}

		Terminal::WriteLine("HP: {}/{}", hitPoints.GetCurrent(), hitPoints.GetMaximum());

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		MenuAction defaultAction = { "Never mind", OnLeave };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void Characteristics::Start()
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