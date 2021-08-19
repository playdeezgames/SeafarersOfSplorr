#pragma once
#include <string>
#include <optional>
#include "Visuals.Data.Type.h"
namespace visuals::data::Types
{
	std::optional<visuals::data::Type> FromString(const std::string&);
}
