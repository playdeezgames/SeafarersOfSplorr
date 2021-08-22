#include "Application.Engine.h"
#include <Audio.h>
#include <Audio.Sfx.h>
#include <Audio.Mux.h>
#include <Data.JSON.Stores.h>
#include "Options.h"
namespace Options
{
	const std::string MUTED = "muted";
	const std::string SFX_VOLUME = "sfxVolume";
	const std::string MUX_VOLUME = "muxVolume";
	const std::string FULLSCREEN = "fullscreen";

	std::optional<int> store;
	void SetStore(int s)
	{
		store = s;
	}

	void Initialize()
	{
		auto properties = data::json::Stores::GetStore(store.value());
		Audio::SetMuted((bool)properties[MUTED]);
		::audio::Sfx::SetVolume((int)properties[SFX_VOLUME]);
		::audio::Mux::SetVolume((int)properties[MUX_VOLUME]);
		bool fullscreen = (properties.count(FULLSCREEN) > 0) ? ((bool)properties[FULLSCREEN]) : (false);
		application::Engine::SetFullscreen(fullscreen);
	}

	void Save()
	{
		auto& properties = data::json::Stores::GetStore(store.value());
		properties[MUTED] = Audio::IsMuted();
		properties[MUX_VOLUME] = ::audio::Mux::GetVolume();
		properties[SFX_VOLUME] = ::audio::Sfx::GetVolume();
		properties[FULLSCREEN] = application::Engine::IsFullscreen();
		data::json::Stores::Save(store.value());
	}
}