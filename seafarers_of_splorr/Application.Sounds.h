#pragma once
#include "Application.UI.Sfx.h"
#include <string>
#include <optional>
namespace application::Sounds//TODO: replace application::ui::Sfx with int, add template version of Read, then move to splorr_application
{
	std::optional<std::string> Read(const application::ui::Sfx&);
}
