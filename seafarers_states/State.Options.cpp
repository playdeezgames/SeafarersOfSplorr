#include <Application.Engine.h>
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Options.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Audio.h>
#include <Audio.Mux.h>
#include <Audio.Sfx.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.Terminal.h"
#include "States.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::OPTIONS;
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const std::string MUTE = "Mute";
	static const std::string UNMUTE = "Unmute";
	static const int VOLUME_DELTA = 8;

	static const std::string SFX_SAMPLE = "woohoo";

	static void AdjustSfxVolume(int delta)
	{
		::audio::Sfx::SetVolume(::audio::Sfx::GetVolume() + delta);
		::audio::Sfx::Play(SFX_SAMPLE);
		application::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		::audio::Mux::SetVolume(::audio::Mux::GetVolume() + delta);
		application::Options::Save();
	}

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Options:");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::WriteLine("Sfx Volume {}",::audio::Sfx::GetVolume());
		Terminal::WriteLine("Mux Volume {}",::audio::Mux::GetVolume());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine();
		if (Audio::IsMuted())
		{
			Terminal::WriteLine("1) Unmute");
		}
		else
		{
			Terminal::WriteLine("1) Mute");
		}
		Terminal::WriteLine("2) Increase SFX");
		Terminal::WriteLine("3) Decrease SFX");
		Terminal::WriteLine("4) Increase MUX");
		Terminal::WriteLine("5) Descease MUX");
		Terminal::WriteLine("6) Toggle Fullscreen");
		Terminal::WriteLine("7) Exit");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void ToggleMute()
	{
		Audio::SetMuted(!Audio::IsMuted()); 
		application::Options::Save();
		Refresh();
	}

	static void ToggleFullscreen()
	{
		application::Engine::SetFullscreen(!application::Engine::IsFullscreen());
		application::Options::Save();
		Refresh();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void IncreaseSfx()
	{
		AdjustSfxVolume(VOLUME_DELTA);
		Refresh();
	}

	static void DecreaseSfx()
	{
		AdjustSfxVolume(-VOLUME_DELTA);
		Refresh();
	}

	static void IncraseMux()
	{
		AdjustMuxVolume(VOLUME_DELTA);
		Refresh();
	}

	static void DecreaseMux()
	{
		AdjustMuxVolume(-VOLUME_DELTA);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions = 
	{
		{"1", ToggleMute},
		{"2", IncreaseSfx},
		{"3", DecreaseSfx},
		{"4", IncraseMux},
		{"5", DecreaseMux},
		{"6", ToggleFullscreen},
		{"7", application::UIState::Pop }
	};

	void Options::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and 6", Refresh));
	}
}