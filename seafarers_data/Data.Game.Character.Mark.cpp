#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Mark.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;

	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterMarks]
		(
			[CharacterId] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[X] REAL NOT NULL,
			[Y] REAL NOT NULL,
			UNIQUE([CharacterId],[Name]));)"s;
	static const std::string QUERY_ITEM =
		R"(SELECT 
			[X],
			[Y] 
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={} 
			AND [Name]={};)"s;
	static const std::string QUERY_ANY =
		R"(SELECT 
			[Name]
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={}
		LIMIT 1;)"s;
	static const std::string QUERY_NAMES =
		R"(SELECT 
			[Name]
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={}
		ORDER BY 
			[Name];)"s;

	static const std::string FIELD_NAME = "Name";

	void Mark::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<common::XY<double>> Mark::ReadMark(int characterId, const std::string& name)
	{
		auto record = Common::TryExecuteForOne(QUERY_ITEM, characterId, common::Data::QuoteString(name));
		if (record)
		{
			return Common::ToXY(record.value());
		}
		return std::nullopt;
	}

	bool Mark::HasAny(int characterId)
	{
		return Common::TryExecuteForOne(QUERY_ANY, characterId).has_value();
	}

	std::list<std::string> Mark::ReadMarkNames(int characterId)
	{
		auto records = Common::Execute(QUERY_NAMES, characterId);
		std::list<std::string> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToString(FIELD_NAME));
		return result;
	}
}