#include "State.InPlay.RenameShipNoun.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include "State.ScratchPad.ShipAdjective.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.ShipNames.h>
namespace state::in_play
{
	std::optional<int> RenameShipNoun::stateId = std::nullopt;

	static std::map<int, std::string> nouns;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Select Noun:");

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static std::function<void()> DoChooseNoun(const std::string& noun)
	{
		return [noun]()
		{
			game::session::Player()
				.GetCharacter()
				.GetBerth()
				.GetShip()
				.SetName(std::format("{} {}", scratch_pad::ShipAdjective::GetAdjective(), noun));
			application::UIState::Write(ShipStatus::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto nouns = game::ShipNames::GetNouns();
		std::for_each(
			nouns.begin(),
			nouns.end(),
			[](const std::string& noun)
			{
				Terminal::menu.AddAction({ noun, DoChooseNoun(noun) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void RenameShipNoun::Start()
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