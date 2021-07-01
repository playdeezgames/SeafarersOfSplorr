#include "Game.World.Data.h"
#include <format>
#include "Game.h"
#include "Game.Data.Properties.h"
namespace game::world::Data
{
	std::string XYToRoomKey(const common::XY<size_t>& xy)
	{
		return std::format("({},{})", xy.GetX(), xy.GetY());
	}

	nlohmann::json& GetNSBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::NS_BORDERS) == 0)
		{
			data[game::data::Properties::NS_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::NS_BORDERS];
	}

	nlohmann::json& GetNSBumps()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::NS_BUMPS) == 0)
		{
			data[game::data::Properties::NS_BUMPS] = nlohmann::json();
		}
		return data[game::data::Properties::NS_BUMPS];
	}

	nlohmann::json& GetEWBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EW_BORDERS) == 0)
		{
			data[game::data::Properties::EW_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::EW_BORDERS];
	}

	nlohmann::json& GetEWBumps()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EW_BUMPS) == 0)
		{
			data[game::data::Properties::EW_BUMPS] = nlohmann::json();
		}
		return data[game::data::Properties::EW_BUMPS];
	}

	nlohmann::json& GetExplored()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EXPLORED) == 0)
		{
			data[game::data::Properties::EXPLORED] = nlohmann::json();
		}
		return data[game::data::Properties::EXPLORED];
	}

	void ClearExplored()
	{
		game::world::Data::GetExplored().clear();
	}
}
