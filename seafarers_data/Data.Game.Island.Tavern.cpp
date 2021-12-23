#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Tavern.h"
namespace data::game::island
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [Taverns]
		(
			[IslandId] INT NOT NULL UNIQUE,
			[TavernName] TEXT NOT NULL
		);)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[IslandId], 
			[TavernName] 
		FROM [Taverns] 
		WHERE 
			[IslandId]={};)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [Taverns]
		(
			[IslandId],
			[TavernName]
		) 
		VALUES({},{});)"s;

	static const std::string FIELD_ISLAND_ID = "IslandId";
	static const std::string FIELD_TAVERN_NAME = "TavernName";

	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	static Tavern ToTavern(const std::map<std::string, std::string>& record)
	{
		return 			
		{
			common::Data::ToInt(record.find(FIELD_ISLAND_ID)->second),
			record.find(FIELD_TAVERN_NAME)->second
		};
	}

	std::optional<Tavern> Tavern::Read(int islandId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, islandId);
		if (!records.empty())
		{
			return ToTavern(records.front());
		}
		return std::nullopt;
	}

	void Tavern::Write(const Tavern& tavern)
	{
		AutoCreateTable();
		Common::Execute(
			REPLACE_ITEM, 
			tavern.islandId, 
			common::Data::QuoteString(tavern.name));
	}

}