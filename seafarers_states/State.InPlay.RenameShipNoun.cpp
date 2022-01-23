#include "State.InPlay.RenameShipNoun.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include "State.ScratchPad.ShipAdjective.h"
#include <algorithm>
#include <Game.Session.Player.h>
#include <Game.ShipNames.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
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
			game::session::character::Berth(game::session::Player::GetCharacterId())
				.GetShip()
				.SetName(std::format("{} {}", scratch_pad::ShipAdjective::GetAdjective(), noun));
			application::UIState::Write(ShipStatus::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto allNouns = game::ShipNames::GetNouns();
		std::for_each(
			allNouns.begin(),
			allNouns.end(),
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