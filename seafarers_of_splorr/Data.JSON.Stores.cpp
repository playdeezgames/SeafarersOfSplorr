#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Data.JSON.Store.h"
#include "Data.JSON.Stores.h"
#include <map>
#include <optional>
#include <string>
#include "Visuals.Texts.h"
namespace data::json::Stores
{
	static std::map<data::json::Store, nlohmann::json> stores;
	static bool modded = false;

	nlohmann::json& GetStore(const data::json::Store& store)
	{
		return stores[store];
	}

	struct storeFile
	{
		std::string filename;
		std::optional<unsigned char> checkSum;
	};

	const std::map<data::json::Store, storeFile> storeFiles =
	{
		{data::json::Store::COLORS, {"config/graphics/colors.json",std::nullopt}},
		{data::json::Store::SOUND_EFFECTS, {"config/audio/sfx.json",std::nullopt}},
		{data::json::Store::MUSIC_THEMES, {"config/audio/mux.json",std::nullopt}},
		{data::json::Store::TEXTURES, {"config/graphics/textures.json",std::nullopt}},
		{data::json::Store::SPRITES, {"config/graphics/sprites.json",std::nullopt}},
		{data::json::Store::FONTS, {"config/graphics/fonts.json",std::nullopt}},
		{data::json::Store::LAYOUTS, {"config/ui/layouts.json",std::nullopt}},
		{data::json::Store::KEYS, {"config/keyboard.json",std::nullopt}},
		{data::json::Store::OPTIONS, {"config/options.json",std::nullopt}},
		{data::json::Store::UI_SFX, {"config/ui/sfx.json",std::nullopt}},
		{data::json::Store::TIPS, {"config/tips.json",std::nullopt}}
	};

	static unsigned char GetFileCheckSum(const std::string& filename)
	{
		unsigned char result = 0;
		FILE* file = nullptr;
		fopen_s(&file, filename.c_str(), "rb");
		unsigned char data;
		if (file)
		{
			while (!feof(file))
			{
				fread(&data, 1, 1, file);
				result += data;
			}
			fclose(file);
		}
		return result;
	}

	void Start()
	{
		for (auto& entry : storeFiles)
		{
			auto checkSum = GetFileCheckSum(entry.second.filename);
			if (entry.second.checkSum.has_value() && checkSum != entry.second.checkSum.value())
			{
				modded = true;
#ifdef _DEBUG
				throw entry.second.filename;
#endif
			}
			stores[entry.first] = ::data::JSON::Load(entry.second.filename);
		}
	}

	void Save(const data::json::Store& store)
	{
		auto iter = storeFiles.find(store);
		if (iter != storeFiles.end())
		{
			data::JSON::Save(iter->second.filename, GetStore(store));
		}
	}

	bool IsModded()
	{
		return modded;
	}
}
