#include "Data.Game.Island.Known.h"
#include <string>
#include "Data.Game.Common.h"
#include <format>
#include "Common.Data.h"
namespace data::game::island::BoundFor
{
	const int AVATAR_ID = 1;//TODO: duplicated
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [BoundForIslands]([AvatarId] INT NOT NULL UNIQUE,[X] REAL NOT NULL, [Y] REAL NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [BoundForIslands] WHERE [AvatarId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [BoundForIslands]([AvatarId],[X],[Y]) VALUES({},{:.4f},{:.4f});";
	const std::string DELETE_ALL = "DELETE FROM [BoundForIslands];";

	static void AutoCreateBoundForIslandsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Write(const common::XY<double>& location)
	{
		AutoCreateBoundForIslandsTable();
		auto query = std::format(REPLACE_ITEM, AVATAR_ID, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}

	std::optional<common::XY<double>> Read()
	{
		AutoCreateBoundForIslandsTable();
		auto query = std::format(QUERY_ITEM, AVATAR_ID);
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
		AutoCreateBoundForIslandsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}