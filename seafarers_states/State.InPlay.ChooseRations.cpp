#include <Common.Data.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.ChooseRations.h"
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHOOSE_RATIONS;

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL);

	static std::vector<std::optional<game::Item>> rationsMenu;

	static void UpdateRationsMenu()
	{
		rationsMenu.clear();
		rationsMenu.push_back(std::nullopt);
		auto allRations = game::Items::AllRations();
		for (auto& entry : allRations)
		{
			rationsMenu.push_back(entry);
		}
	}


	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		auto avatarId = GetCrewDetailCharacterId();
		Terminal::WriteLine("Rations for {}:", game::Session().GetCharacters().GetCharacter(avatarId).GetName());

		Terminal::SetForeground(game::Colors::YELLOW);

		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateRationsMenu();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < rationsMenu.size())
		{
			OnLeave();
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void ChooseRations::Start()
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
				OnOtherInput));
	}
}