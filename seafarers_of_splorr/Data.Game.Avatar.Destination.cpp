#include "Data.Game.Island.Known.h"
#include <string>
#include "Data.Game.Common.h"
#include <format>
#include "Common.Data.h"
namespace data::game::avatar::Destination
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDestinations]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL, [Y] REAL NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [AvatarDestinations] WHERE [AvatarId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDestinations]([AvatarId],[X],[Y]) VALUES({},{:.4f},{:.4f});";
	const std::string DELETE_ALL = "DELETE FROM [AvatarDestinations];";

	static void AutoCreateAvatarDestinationsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const common::XY<double>& location)
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}

	std::optional<common::XY<double>> Read()
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(QUERY_ITEM, data::game::Common::AVATAR_ID);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<common::XY<double>>(
				{
					common::Data::StringToDouble(record[FIELD_X]),
					common::Data::StringToDouble(record[FIELD_Y])
				});
		}
		return std::nullopt;
	}

	void Clear()
	{
		AutoCreateAvatarDestinationsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}