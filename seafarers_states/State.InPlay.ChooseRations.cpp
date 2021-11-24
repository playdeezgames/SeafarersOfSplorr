#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Items.h>
#include <Game.Character.Rations.h>
#include <Game.Colors.h>
#include <Game.Items.h>
#include "State.InPlay.ChooseRations.h"
#include "State.InPlay.CrewDetail.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <vector>
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
		auto avatarId = CrewDetail::GetAvatarId();
		Terminal::WriteLine("Rations for {}:", game::Character::GetName(avatarId).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		auto current = game::character::Rations::Read(avatarId);

		int index = 1;
		for (auto& entry : rationsMenu)
		{
			std::string name = entry.has_value() ? game::Items::GetName(entry.value()) : "(nothing)";
			Terminal::Write("{}) {}", index++, name);
			if (entry == current)
			{
				Terminal::Write("*");
			}
			if (entry.has_value())
			{
				Terminal::Write("(cargo: {})", game::character::Items::Read(avatarId, entry.value()));
			}
			Terminal::WriteLine();
		}

		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
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
			game::character::Rations::Write(CrewDetail::GetAvatarId(), rationsMenu[index]);
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