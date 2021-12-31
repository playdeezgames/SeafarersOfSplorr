#include <Common.Data.h>
#include "Data.Game.Skill.h"
#include "Data.Game.Common.h"
namespace data::game
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Skills]
		(
			[SkillId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Category] INT NOT NULL,
			[Type] INT NOT NULL,
			[Name] TEXT NOT NULL
		);)"s;
	static const std::string QUERY_NEXT_TYPE_FOR_CATEGORY =
		R"(SELECT 
			MAX([Type]) AS [MaximumType]
		FROM [Skills] 
		WHERE 
			[Category]={};)"s;
	static const std::string INSERT_ITEM =
		R"(INSERT INTO [Skills]
		(
			[Category],
			[Type],
			[Name]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Skills] 
		WHERE 
			[SkillId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [Skills];)"s;

	static const std::string FIELD_MAXIMUM_TYPE = "MaximumType";
	static const std::string FIELD_CATEGORY = "Category";
	static const std::string FIELD_TYPE = "Type";
	static const std::string FIELD_NAME = "Name";

	void Skill::Initialize()
	{
		Common::Execute(CREATE_TABLE);
	}

	int Skill::ReadNextTypeForCategory(int category)
	{
		Initialize();
		return
			Common::TryToInt(
				Common::TryExecuteForOne(QUERY_NEXT_TYPE_FOR_CATEGORY, category),
				FIELD_MAXIMUM_TYPE).value_or(0) + 1;
	}

	int Skill::EstablishTypeForCategory(int category, int type, const std::string& name)
	{
		Initialize();
		Common::Execute(
			INSERT_ITEM, 
			category, 
			type, 
			common::Data::QuoteString(name));
		return Common::LastInsertedIndex();
	}

	std::optional<int> Skill::ReadCategory(int skillId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_CATEGORY, skillId),
			FIELD_CATEGORY);
	}

	std::optional<int> Skill::ReadType(int skillId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_TYPE, skillId),
			FIELD_TYPE);
	}

	std::optional<std::string> Skill::ReadName(int skillId)
	{
		Initialize();
		return Common::TryToString(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_NAME, skillId),
			FIELD_NAME);
	}

	void Skill::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}