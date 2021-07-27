#include "Data.JSON.h"
#include "Data.Stores.h"
#include <map>
#include <vector>
#include "Visuals.Colors.h"
#include "Visuals.Data.Properties.h"
namespace visuals::Colors
{
	static std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> ParseDescriptor(const nlohmann::json& properties)
	{
		unsigned char r = (unsigned char)properties[data::Properties::RED];
		unsigned char g = (unsigned char)properties[data::Properties::GREEN];
		unsigned char b = (unsigned char)properties[data::Properties::BLUE];
		unsigned char a = (unsigned char)properties[data::Properties::ALPHA];
		return { r,g,b,a };
	}

	std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> Read(const std::string& key)
	{
		return ParseDescriptor(::data::Stores::GetStore(::data::Store::COLORS)[key]);
	}
}