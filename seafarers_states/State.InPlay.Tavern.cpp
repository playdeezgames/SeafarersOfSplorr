#include <Game.Characters.Islands.h>
#include <Game.Islands.Taverns.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.Tavern.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_TAVERN;

	static void RefreshCharacters(int islandId)
	{
		auto characterIds = game::characters::Islands::All(islandId, game::characters::State::TAVERN);
		if (!characterIds.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("People Present:");
			for (auto characterId : characterIds)
			{
				bool isPC = characterId == GetPlayerCharacterId();
				auto name = game::Session().GetCharacters().GetCharacter(characterId).GetName();
				if (isPC)
				{
					Terminal::WriteLine("{}(you)", name);
				}
				else
				{
					Terminal::WriteLine(name);
				}
			}
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto islandId = GetPlayerCharacterIslandId().value();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(
			"Yer in a tavern called 'the {}'.",
			game::islands::Taverns::GetName(islandId).value());

		RefreshCharacters(islandId);

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();

	}

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave }
	};

	void Tavern::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}
