#include <SDL.h> //legit
#include <string>
#include <optional>
#include "Application.Renderer.h"
#include "Visuals.Colors.h"
#include "Visuals.Textures.h"
#include "Visuals.Sprites.h"
#include "Visuals.Fonts.h"
#include "Visuals.Layouts.h"
#include "Common.Audio.h"
#include "Options.h"
#include <map>
#include "Application.MouseMotion.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Common.Utility.h"
#include "Application.Keyboard.h"
#include "States.h"
#include "Sublayouts.h"
#include "Data.Stores.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Game.Achievements.h"
namespace Application
{
	static std::optional<::Command> KeyCodeToCommand(int code)
	{
		return application::Keyboard::ToCommand(code);
	}

	static void HandleKeyDown(const SDL_KeyboardEvent& evt)
	{
		auto command = application::Keyboard::ToCommand((int)evt.keysym.sym);
		if (command)
		{
			application::Command::Handle(command.value());
		}
	}

	static std::map<Uint8, Command> controllerButtonCommand = 
	{
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Command::DOWN     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_UP,       Command::UP       },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Command::LEFT     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Command::RIGHT    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_A,             Command::GREEN    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_B,             Command::RED      },
		{ (Uint8)SDL_CONTROLLER_BUTTON_X,             Command::BLUE     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_Y,             Command::YELLOW   },
		{ (Uint8)SDL_CONTROLLER_BUTTON_BACK,          Command::BACK     },
		{ (Uint8)SDL_CONTROLLER_BUTTON_START,         Command::START    },
		{ (Uint8)SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  Command::PREVIOUS },
		{ (Uint8)SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Command::NEXT     }
	};

	static void HandleControllerButtonDown(const SDL_ControllerButtonEvent& evt)
	{
		auto item = controllerButtonCommand.find(evt.button);
		if (item!=controllerButtonCommand.end())
		{
			::application::Command::Handle(item->second);
		}
	}
}
namespace common::Application
{
	static std::vector<std::function<void()>> starters = 
	{
		game::Achievements::Start,
		data::Stores::Start,
		state::Splash::Start,
		state::Tip::Start,
		state::MainMenu::Start,
		state::About::Start,
		state::ConfirmQuit::Start,
		state::Options::Start,
		state::Start::Start,
		state::LeavePlay::Start,
		game::Start,
		state::Statistics::Start,
		state::LoadGame::Start,
		state::SaveGame::Start,
		state::ConfirmAbandon::Start,
		state::ConfirmOverwrite::Start,
		state::in_play::AtSea::Start,
		state::in_play::ChangeSpeed::Start,
		state::in_play::Docked::Start,
		state::in_play::Win::Start,
		state::in_play::Lose::Start,
		state::in_play::HeadFor::Start,
		state::in_play::IslandJobs::Start,
		state::in_play::ConfirmReplaceJob::Start,
		state::in_play::ConfirmAbandonJob::Start,
		state::in_play::CurrentJob::Start,
		sublayout::UIHamburger::Start,
		Options::Initialize,
		visuals::Layouts::Start
	};

	void Start(const std::vector<std::string>& arguments)
	{
		for (auto& starter : starters)
		{
			starter();
		}
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	bool IsRunning()
	{
		return ::application::UIState::Read() != ::UIState::QUIT;
	}

	static std::optional<::UIState> currentState = std::nullopt;

	void Update(unsigned int ticks)
	{
		if (!currentState || currentState.value() != ::application::UIState::Read())
		{
			currentState = ::application::UIState::Read();
			//load the layout!
			application::OnEnter::Handle();
		}
		application::Update::Handle(ticks);
	}

	std::map<unsigned int, std::function<void(const SDL_Event&)>> eventHandlers =
	{
		{ SDL_QUIT,                 [](const SDL_Event&    ) { ::application::UIState::Write(::UIState::QUIT); }},
		{ SDL_KEYDOWN,              [](const SDL_Event& evt) { ::Application::HandleKeyDown(evt.key); }},
		{ SDL_CONTROLLERBUTTONDOWN, [](const SDL_Event& evt) { ::Application::HandleControllerButtonDown(evt.cbutton); }},
		{ SDL_MOUSEMOTION,          [](const SDL_Event& evt) { ::application::MouseMotion::Handle(evt.motion.x, evt.motion.y); }},
		{ SDL_MOUSEBUTTONUP,        [](const SDL_Event& evt) { ::application::MouseButtonUp::Handle(evt.button.x, evt.button.y, evt.button.button); }}
	};

	void HandleEvent(const SDL_Event& evt)
	{
		auto item = eventHandlers.find(evt.type);
		if (item!=eventHandlers.end())
		{
			item->second(evt);
		}
	}
}