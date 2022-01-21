#include "Data.Game.Character.Mark.h"
#include "Data.Game.Character.h"
#include "Data.Game.Common.h"
#include <algorithm>
#include <Common.Data.h>
namespace data::game::character
{
	using namespace std::string_view_literals;

	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [CharacterMarks]
		(
			[CharacterId] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[X] REAL NOT NULL,
			[Y] REAL NOT NULL,
			UNIQUE([CharacterId],[Name]));)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[X],
			[Y] 
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={} 
			AND [Name]={};)"sv;
	static constexpr std::string_view QUERY_ANY =
		R"(SELECT 
			[Name]
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={}
		LIMIT 1;)"sv;
	static constexpr std::string_view QUERY_NAMES =
		R"(SELECT 
			[Name]
		FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={}
		ORDER BY 
			[Name];)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterMarks]
		(
			[CharacterId],
			[Name],
			[X],
			[Y]
		) 
		VALUES({},{},{},{});)"sv;
	static constexpr std::string_view DELETE_ITEM =
		R"(DELETE FROM [CharacterMarks] 
		WHERE 
			[CharacterId]={} 
			AND [Name]={};)"sv;

	static constexpr std::string_view FIELD_NAME = "Name";

	void Mark::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	std::optional<common::XY<double>> Mark::ReadMark(int characterId, const std::string_view& name)
	{
		Initialize();
		auto record = Common::TryExecuteForOne(
			QUERY_ITEM, 
			characterId, 
			common::Data::QuoteString(std::string(name)));
		if (record)
		{
			return Common::ToXY(record.value());
		}
		return std::nullopt;
	}

	bool Mark::HasAny(int characterId)
	{
		Initialize();
		return Common::TryExecuteForOne(QUERY_ANY, characterId).has_value();
	}

	std::vector<std::string> Mark::ReadMarkNames(int characterId)
	{
		Initialize();
		auto records = Common::Execute(QUERY_NAMES, characterId);
		std::vector<std::string> result;
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToString(FIELD_NAME));
		return result;
	}

	void Mark::WriteMark(int characterId, const std::string& name, const common::XY<double>& location)
	{
		Initialize();
		Common::Execute(
			REPLACE_ITEM, 
			characterId, 
			common::Data::QuoteString(name), 
			location.GetX(), 
			location.GetY());
	}

	void Mark::ClearMark(int characterId, const std::string& name)
	{
		Initialize();
		Common::Execute(
			DELETE_ITEM, 
			characterId, 
			common::Data::QuoteString(name));
	}
}