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
#include "State.Options.h"
#include "State.Registrar.h"
#include "State.Terminal.h"
namespace state
{
	std::optional<int> Options::stateId = std::nullopt;
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
		if (Audio::IsMuted())
		{
			Terminal::WriteLine("==MUTED==");
		}
		else
		{
			Terminal::WriteLine("Sfx Volume {}%",::audio::Sfx::GetVolumePercentage());
			Terminal::WriteLine("Mux Volume {}%",::audio::Mux::GetVolumePercentage());
		}

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Toggle mute");
		Terminal::WriteLine("2) Increase SFX");
		Terminal::WriteLine("3) Decrease SFX");
		Terminal::WriteLine("4) Increase MUX");
		Terminal::WriteLine("5) Descease MUX");
		Terminal::WriteLine("6) Toggle fullscreen");
		Terminal::WriteLine("0) Never mind");

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
		{"0", application::UIState::Pop }
	};

	void Options::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId, 
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId, 
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId, 
					Terminal::DoIntegerInput(
						menuActions, 
						Terminal::INVALID_INPUT, 
						Refresh));
			});
	}
}