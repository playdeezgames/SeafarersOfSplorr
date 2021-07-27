#include "Common.Application.h"
#include "Common.Audio.h"
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
		common::Audio::SetMuted((bool)properties[MUTED]);
		common::audio::Sfx::SetVolume((int)properties[SFX_VOLUME]);
		common::audio::Mux::SetVolume((int)properties[MUX_VOLUME]);
		bool fullscreen = (properties.count(FULLSCREEN) > 0) ? ((bool)properties[FULLSCREEN]) : (false);
		common::Application::SetFullscreen(fullscreen);
	}

	void Save()
	{
		auto& properties = data::Stores::GetStore(data::Store::OPTIONS);
		properties[MUTED] = common::Audio::IsMuted();
		properties[MUX_VOLUME] = common::audio::Mux::GetVolume();
		properties[SFX_VOLUME] = common::audio::Sfx::GetVolume();
		properties[FULLSCREEN] = common::Application::IsFullscreen();
		data::Stores::Save(data::Store::OPTIONS);
	}
}