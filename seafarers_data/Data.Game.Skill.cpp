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
			[Name] TEXT NOT NULL,
			[DefaultValue] INT NOT NULL
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
			[Name],
			[DefaultValue]
		) 
		VALUES({},{},{},{});)"s;
	static const std::string QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Skills] 
		WHERE 
			[SkillId]={};)"s;
	static const std::string DELETE_ALL =
		R"(DELETE FROM [Skills];)"s;
	static const std::string QUERY_SKILL_FOR_CATEGORY_AND_TYPE =
		R"(SELECT 
			[SkillId] 
		FROM [Skills] 
		WHERE 
			[Category]={} 
			AND [Type]={})"s;

	static const std::string FIELD_MAXIMUM_TYPE = "MaximumType";
	static const std::string FIELD_CATEGORY = "Category";
	static const std::string FIELD_TYPE = "Type";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_SKILL_ID = "SkillId";
	static const std::string FIELD_DEFAULT_VALUE = "DefaultValue";

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

	int Skill::EstablishTypeForCategory(int category, int type, const std::string& name, int defaultValue)
	{
		Initialize();
		Common::Execute(
			INSERT_ITEM, 
			category, 
			type, 
			common::Data::QuoteString(name),
			defaultValue);
		return Common::LastInsertedIndex();
	}

	static const int GENERAL_TYPE = 0;

	int Skill::EstablishGeneralSkillForCategory(int category, const std::string& name, int defaultValue)
	{
		Initialize();
		return EstablishTypeForCategory(category, GENERAL_TYPE, name, defaultValue);
	}

	std::optional<int> Skill::ReadGeneralSkillForCategory(int category)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_SKILL_FOR_CATEGORY_AND_TYPE, category, GENERAL_TYPE),
			FIELD_SKILL_ID);
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

	std::optional<int> Skill::ReadDefaultValue(int skillId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM_COLUMN, FIELD_DEFAULT_VALUE, skillId),
			FIELD_NAME);
	}

	void Skill::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}
}