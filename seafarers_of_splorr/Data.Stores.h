#pragma once
#include "json.hpp"
namespace data
{
	enum class Store
	{
		COLORS,
		SOUND_EFFECTS,
		MUSIC_THEMES,
		TEXTURES,
		SPRITES,
		FONTS,
		LAYOUTS,
		KEYS,
		OPTIONS,
		UI_SFX,
		TIPS
	};
}
namespace data::Stores
{
	nlohmann::json& GetStore(const data::Store&);
	void Start();
	void Save(const data::Store&);
	bool IsModded();
}
