#pragma once
#include "Application.UI.Sfx.h"
#include <string>
#include <optional>
namespace application::Sounds
{
	std::optional<std::string> Read(const application::ui::Sfx&);
}
