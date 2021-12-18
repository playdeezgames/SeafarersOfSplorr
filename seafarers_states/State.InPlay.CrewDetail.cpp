#include <Game.Characters.Rations.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL;

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST);

	static void RefreshRations(int characterId)
	{
		auto rations = game::characters::Rations::Read(characterId);
		if (rations)
		{
			Terminal::WriteLine("Rations: {}", game::Items::GetName(rations.value()));
		}
		else
		{
			Terminal::WriteLine("Rations: (nothing)");
		}
	}

	static void RefreshFlags(int characterId)
	{
		auto character =
			game::Session()
			.GetCharacters()
			.GetCharacter(characterId);
		auto flags = character.GetFlags().GetAll();
		if (!flags.empty())
		{
			bool first = true;
			for (auto flag : flags)
			{
				auto& name = flag.GetName();
				if (first)
				{
					Terminal::Write("Status: {}", name);
				}
				else
				{
					Terminal::Write(", {}", name);
				}
				first = false;
			}
			Terminal::WriteLine();
		}
	}

	static void RefreshCharacteristics(int characterId)
	{
		Terminal::WriteLine("Maximum HP: {}", game::Session().GetCharacters().GetCharacter(characterId).GetHitpoints().GetMaximum());
	}

	static void Refresh()
	{
		int characterId = GetCrewDetailCharacterId();

		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", game::Session().GetCharacters().GetCharacter(characterId).GetName());
		RefreshRations(characterId);
		RefreshFlags(characterId);
		RefreshCharacteristics(characterId);//TODO: make this its own screen

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Characteristics");
		Terminal::WriteLine("2) Skills");
		Terminal::WriteLine("3) Statistics");
		Terminal::WriteLine("4) Equipment");
		Terminal::WriteLine("5) Change Rations");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL_CHARACTERISTICS) },
		{ "3", application::UIState::GoTo(::UIState::IN_PLAY_CHARACTER_STATUS) },
		{ "4", application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)},
		{ "5", application::UIState::GoTo(::UIState::IN_PLAY_CHOOSE_RATIONS)},
		{ "0", OnLeave}
	};

	void CrewDetail::Start()
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