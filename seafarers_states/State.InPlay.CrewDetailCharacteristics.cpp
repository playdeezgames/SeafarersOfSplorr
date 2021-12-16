#include <Game.Session.h>
#include "State.InPlay.CrewDetailCharacteristics.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL_CHARACTERISTICS;

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static void Refresh()
	{
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(GetCrewDetailCharacterId());
		auto characteristics =
			character
			.GetCharacteristicsLegacy();
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

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Done");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	void CrewDetailCharacteristics::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE,
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE,
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}