#include <algorithm>
#include "Application.UIState.h"
#include "Command.h"
#include <functional>
#include <map>
#include "MouseButton.h"
#include <optional>
#include <SDL.h>
#include <vector>
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
namespace application::TextInput
{
	void Handle(const std::string&);
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
namespace application::UIState
{
	extern std::optional<int> finalState;
}
namespace application::Engine
{
	extern std::vector<std::function<void()>> starters;

	void Start(const std::vector<std::string>& arguments)
	{
		std::for_each(starters.begin(), starters.end(), [](auto p) { p(); });
	}

	bool IsRunning()
	{
		return ::application::UIState::Read() != application::UIState::finalState.value();
	}

	static std::optional<int> currentState = std::nullopt;

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
		{ SDL_QUIT,                 [](const SDL_Event&    ) { ::application::UIState::Write(application::UIState::finalState.value()); }},
		{ SDL_KEYDOWN,              [](const SDL_Event& evt) { ::Application::HandleKeyDown(evt.key); }},
		{ SDL_CONTROLLERBUTTONDOWN, [](const SDL_Event& evt) { ::Application::HandleControllerButtonDown(evt.cbutton); }},
		{ SDL_MOUSEMOTION,          [](const SDL_Event& evt) { ::application::MouseMotion::Handle(evt.motion.x, evt.motion.y); }},
		{ SDL_MOUSEBUTTONUP,        [](const SDL_Event& evt) { ::application::MouseButtonUp::Handle(evt.button.x, evt.button.y,mouseButtonTable.find(evt.button.button)->second); }},
		{ SDL_TEXTINPUT,            [](const SDL_Event& evt) { ::application::TextInput::Handle(evt.text.text); }}
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