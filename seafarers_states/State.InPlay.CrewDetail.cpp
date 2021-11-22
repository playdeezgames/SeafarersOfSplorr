#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Flags.h>
#include <Game.Character.Rations.h>
#include <Game.Colors.h>
#include <Game.Items.h>
#include "State.InPlay.CrewDetail.h"
#include "State.Terminal.h"
#include <string>
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL;

	static int currentAvatarId;

	void CrewDetail::SetAvatarId(int avatarId)
	{
		currentAvatarId = avatarId;
	}

	int CrewDetail::GetAvatarId()
	{
		return currentAvatarId;
	}

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST);

	static void RefreshRations()
	{
		auto rations = game::character::Rations::Read(currentAvatarId);
		if (rations)
		{
			Terminal::WriteLine("Rations: {}", game::Items::GetName(rations.value()));
		}
		else
		{
			Terminal::WriteLine("Rations: (nothing)");
		}
	}

	static void RefreshFlags()
	{
		auto flags = game::character::Flags::All(currentAvatarId);
		if (!flags.empty())
		{
			bool first = true;
			for (auto flag : flags)
			{
				auto& name = game::character::Flags::GetName(flag);
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

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Crew Details:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", game::Character::GetName(currentAvatarId).value());
		RefreshRations();
		RefreshFlags();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Statistics");
		Terminal::WriteLine("2) Equipment");
		Terminal::WriteLine("3) Change Rations");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", application::UIState::GoTo(::UIState::IN_PLAY_AVATAR_STATUS) },
		{ "2", application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)},
		{ "3", application::UIState::GoTo(::UIState::IN_PLAY_CHOOSE_RATIONS)},
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