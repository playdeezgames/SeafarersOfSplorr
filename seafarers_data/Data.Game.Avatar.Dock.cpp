#include <Common.Data.h>
#include "Data.Game.Avatar.Dock.h"
#include "Data.Game.Common.h"
#include <format>
#include <string>
namespace data::game::avatar
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDocks]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL,[Y] REAL NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [X], [Y] FROM [AvatarDocks] WHERE [AvatarId] = {};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDocks]([AvatarId],[X],[Y]) VALUES({},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [AvatarDocks] WHERE [AvatarId] = {};";

	const auto AutoCreateAvatarDockTable = data::game::Common::Run(CREATE_TABLE);

	void Dock::ClearLocation()
	{
		AutoCreateAvatarDockTable();
		std::string query =
			(std::format(DELETE_ITEM, data::game::Common::AVATAR_ID));
		data::game::Common::Execute(query);
	}


	void Dock::SetLocation(const common::XY<double>& location)
	{
		AutoCreateAvatarDockTable();
		std::string query =
			std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}

	std::optional<common::XY<double>> Dock::GetLocation()
	{
		AutoCreateAvatarDockTable();
		std::string query = std::format(QUERY_ITEM, data::game::Common::AVATAR_ID);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<common::XY<double>>(
				{
					common::Data::ToDouble(record[FIELD_X]),
					common::Data::ToDouble(record[FIELD_Y])
				});
		}
		return std::nullopt;
	}
}