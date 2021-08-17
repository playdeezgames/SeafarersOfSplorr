#include "Audio.h"
#include "Audio.Sfx.h"
#include "Audio.Mux.h"
#include "Common.Application.h"
#include "Data.Stores.h"
#include "Options.h"
namespace Options
{
	const std::string MUTED = "muted";
	const std::string SFX_VOLUME = "sfxVolume";
	const std::string MUX_VOLUME = "muxVolume";
	const std::string FULLSCREEN = "fullscreen";

	void Initialize()
	{
		auto properties = data::Stores::GetStore(data::Store::OPTIONS);
		Audio::SetMuted((bool)properties[MUTED]);
		::audio::Sfx::SetVolume((int)properties[SFX_VOLUME]);
		::audio::Mux::SetVolume((int)properties[MUX_VOLUME]);
		bool fullscreen = (properties.count(FULLSCREEN) > 0) ? ((bool)properties[FULLSCREEN]) : (false);
		common::Application::SetFullscreen(fullscreen);
	}

	void Save()
	{
		auto& properties = data::Stores::GetStore(data::Store::OPTIONS);
		properties[MUTED] = Audio::IsMuted();
		properties[MUX_VOLUME] = ::audio::Mux::GetVolume();
		properties[SFX_VOLUME] = ::audio::Sfx::GetVolume();
		properties[FULLSCREEN] = common::Application::IsFullscreen();
		data::Stores::Save(data::Store::OPTIONS);
	}
}