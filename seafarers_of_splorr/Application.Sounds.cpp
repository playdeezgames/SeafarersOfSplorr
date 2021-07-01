#include "Application.Sounds.h"
#include "Data.Stores.h"
#include <format>
namespace application::Sounds
{
	static std::string SfxToString(const application::UI::Sfx& sfx)
	{
		return std::format("{}", (int)sfx);
	}

	std::optional<std::string> Read(const application::UI::Sfx& sfx)
	{
		auto& store = data::Stores::GetStore(data::Store::UI_SFX);
		auto id = SfxToString(sfx);
		if (store.count(id) > 0)
		{
			return std::optional<std::string>(store[id]);
		}
		return std::nullopt;
	}
}