#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Quest.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include "State.Terminal.h"
#include "State.InPlay.AtSeaOverview.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("At Sea:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Heading: {:.2f}, Speed: {:.2f}", game::Ship::GetHeading(), game::Ship::GetSpeed());
		//TODO: nearby islands
		auto nearby = game::Islands::GetViewableIslands();
		if (!nearby.empty())
		{
			Terminal::WriteLine("Nearby islands:");
			for (auto& island : nearby)
			{
				Terminal::WriteLine(" {} ({:.2f} dist {:.1f})", 
					island.visits.has_value() ?
					island.name : "????", common::Heading::XYToDegrees(island.relativeLocation), island.relativeLocation.GetMagnitude());
			}
		}
		//TODO: wind from

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Move");
		Terminal::WriteLine("2) Change Heading/Speed");
		if (!game::Islands::GetDockableIslands().empty())
		{
			Terminal::WriteLine("3) Dock/careen");
		}
		Terminal::WriteLine("4) Crew status");
		Terminal::WriteLine("5) Ship status");
		Terminal::WriteLine("6) Menu");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnMove()
	{
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine();
		Terminal::WriteLine("Steady as she goes!");
		game::Ship::Move();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDock()
	{
		if (!game::Islands::GetDockableIslands().empty())
		{
			Terminal::SetForeground(game::Colors::RED);
			Terminal::WriteLine();
			Terminal::WriteLine("TODO: dock");
		}
		else
		{
			Terminal::SetForeground(game::Colors::RED);
			Terminal::WriteLine();
			Terminal::WriteLine("There is no place to dock or careen!");
		}
		Refresh();
	}

	static void OnCrewStatus()
	{
		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine();
		Terminal::WriteLine("TODO: crew status");
		Refresh();
	}

	static void OnShipStatus()
	{
		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine();
		Terminal::WriteLine("TODO: ship status");
		Refresh();
	}

	static void OnMenu()
	{
		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine();
		Terminal::WriteLine("TODO: menu status");
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnMove },
		{"2", application::UIState::GoTo(::UIState::IN_PLAY_STEER_SHIP) },
		{"3", OnDock },
		{"4", OnCrewStatus },
		{"5", OnShipStatus },
		{"6", OnMenu }

		//delete this later
		, {"0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_DEPRECATED)}
	};

	void AtSeaOverview::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				"Please enter a number between 1 and X", 
				Refresh));
	}
}