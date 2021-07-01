#pragma once
#include <string>
#include <optional>
namespace application::UI
{
	enum class Sfx
	{
		WOOHOO
	};
}

namespace application::Sounds
{
	std::optional<std::string> Read(const application::UI::Sfx&);
}
