#include "Application.UIState.h"
#include "Audio.Mux.h"
#include "Audio.Sfx.h"
#include "Command.h"
#include "Data.JSON.Store.h"
#include "Data.JSON.Stores.h"
#include "Data.SQLite.Store.h"
#include "Data.SQLite.Stores.h"
#include "Game.h"
#include "Game.Achievements.h"
#include "Game.Audio.Mux.h"
#include "MouseButton.h"
#include <optional>
#include "Options.h"
#include <SDL.h> //legit
#include "States.h"
#include "Sublayouts.h"
#include "UIState.h"
#include "Visuals.Layouts.h"
#include "Visuals.Colors.h"
#include "Visuals.Textures.h"
namespace application::Command
{
	void Handle(const ::Command& command);
}
namespace application::Keyboard
{
	std::optional<::Command> ToCommand(int);
}
namespace application::MouseButtonUp
{
	void Handle(const int&, const int&, const MouseButton&);
}
namespace application::MouseMotion
{
	void Handle(const int&, const int&);
}
namespace application::OnEnter
{
	void Handle();
}
namespace application::Update
{
	void Handle(unsigned int);
}
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
namespace application::Engine
{
	static std::vector<std::function<void()>> starters = 
	{
		game::Achievements::Start,
		data::json::Stores::DoSetStoreFile(data::json::Store::COLORS, "config/graphics/colors.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SOUND_EFFECTS, "config/audio/sfx.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::MUSIC_THEMES, "config/audio/mux.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TEXTURES, "config/graphics/textures.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SPRITES, "config/graphics/sprites.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::FONTS, "config/graphics/fonts.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::LAYOUTS, "config/ui/layouts.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::KEYS, "config/keyboard.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::OPTIONS, "config/options.json",std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TIPS, "config/tips.json",std::nullopt),
		visuals::Colors::DoSetStore(data::json::Store::COLORS),
		visuals::Textures::DoSetStore(data::json::Store::TEXTURES),
		data::json::Stores::Start,
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::IN_MEMORY, ":memory:"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::AUTOSAVE, "autosave.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_1, "slot1.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_2, "slot2.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_3, "slot3.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_4, "slot4.db"),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_5, "slot5.db"),
		audio::Mux::DoSetStore(data::json::Store::MUSIC_THEMES),
		audio::Sfx::DoSetStore(data::json::Store::SOUND_EFFECTS),
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
		state::in_play::IslandTrade::Start,
		state::in_play::IslandBuy::Start,
		state::in_play::IslandSell::Start,
		state::in_play::Cargo::Start,
		state::in_play::ShipStatus::Start,
		state::in_play::Next::Start,
		state::in_play::Shipyard::Start,
		state::in_play::DarkAlleyEntrance::Start,
		state::in_play::DarkAlley::Start,
		state::in_play::Message::Start,
		state::in_play::Confirm::Start,
		sublayout::UIHamburger::Start,
		Options::Initialize,
		visuals::Layouts::Start
	};

	void Start(const std::vector<std::string>& arguments)
	{
		std::for_each(starters.begin(), starters.end(), [](auto p) { p(); });
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	bool IsRunning()
	{
		return (::UIState)::application::UIState::Read() != ::UIState::QUIT;
	}

	static std::optional<::UIState> currentState = std::nullopt;

	void Update(unsigned int ticks)
	{
		if (!currentState || currentState.value() != (::UIState)::application::UIState::Read())
		{
			currentState = (::UIState)::application::UIState::Read();
			//load the layout!
			application::OnEnter::Handle();
		}
		application::Update::Handle(ticks);
	}

	const std::map<Uint8, MouseButton> mouseButtonTable =
	{
		{SDL_BUTTON_LEFT, MouseButton::LEFT},
		{SDL_BUTTON_MIDDLE, MouseButton::MIDDLE},
		{SDL_BUTTON_RIGHT, MouseButton::RIGHT},
		{SDL_BUTTON_X1, MouseButton::X1},
		{SDL_BUTTON_X2, MouseButton::X2}
	};

	const std::map<unsigned int, std::function<void(const SDL_Event&)>> eventHandlers =
	{
		{ SDL_QUIT,                 [](const SDL_Event&    ) { ::application::UIState::Write(::UIState::QUIT); }},
		{ SDL_KEYDOWN,              [](const SDL_Event& evt) { ::Application::HandleKeyDown(evt.key); }},
		{ SDL_CONTROLLERBUTTONDOWN, [](const SDL_Event& evt) { ::Application::HandleControllerButtonDown(evt.cbutton); }},
		{ SDL_MOUSEMOTION,          [](const SDL_Event& evt) { ::application::MouseMotion::Handle(evt.motion.x, evt.motion.y); }},
		{ SDL_MOUSEBUTTONUP,        [](const SDL_Event& evt) { ::application::MouseButtonUp::Handle(evt.button.x, evt.button.y,mouseButtonTable.find(evt.button.button)->second); }}
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