#include "Data.Game.Island.Item.h"
#include "Data.Game.Common.h"
#include "Common.Data.h"
#include <format>
namespace data::game::island::Item
{
	const std::string FIELD_X = "X";
	const std::string FIELD_Y = "Y";
	const std::string FIELD_ITEM_ID = "ItemId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandItems]([X],[Y],[ItemId], UNIQUE([X],[Y],[ItemId]));";
	const std::string REPLACE_ITEM = "REPLACE INTO [IslandItems]([X],[Y],[ItemId]) VALUES({:.4f},{:.4f},{});";
	const std::string DELETE_ITEM = "DELETE FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f} AND [ItemId]={};";
	const std::string DELETE_ALL = "DELETE FROM [IslandItems];";
	const std::string QUERY_ITEM = "SELECT [ItemId] FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f} AND [ItemId]={};";
	const std::string QUERY_ALL = "SELECT [ItemId] FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f}";

	static void AutoCreateIslandItemTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Set(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, location.GetX(), location.GetY(), item));
	}

	void Clear(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, location.GetX(), location.GetY(), item));
	}

	bool Get(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, location.GetX(), location.GetY(), item));
		return !records.empty();
	}

	void ClearAll()
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::set<int> GetAll(const common::XY<double>& location)
	{
		std::set<int> result;
		AutoCreateIslandItemTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ALL, location.GetX(), location.GetY()));
		for (auto& record : records)
		{
			result.insert(common::Data::StringToInt(record[FIELD_ITEM_ID]));
		}
		return result;
	}
}
