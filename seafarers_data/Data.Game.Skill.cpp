#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Skill.h"
#include "Data.Game.Common.h"
namespace data::game
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [Skills]
		(
			[SkillId] INTEGER PRIMARY KEY AUTOINCREMENT,
			[Category] INT NOT NULL,
			[Type] INT NOT NULL,
			[Name] TEXT NOT NULL,
			[DefaultValue] INT NOT NULL
		);)"sv;
	static constexpr std::string_view QUERY_NEXT_TYPE_FOR_CATEGORY =
		R"(SELECT 
			MAX([Type]) AS [MaximumType]
		FROM [Skills] 
		WHERE 
			[Category]={};)"sv;
	static constexpr std::string_view INSERT_ITEM =
		R"(INSERT INTO [Skills]
		(
			[Category],
			[Type],
			[Name],
			[DefaultValue]
		) 
		VALUES({},{},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM_COLUMN =
		R"(SELECT 
			[{}] 
		FROM [Skills] 
		WHERE 
			[SkillId]={};)"sv;
	static constexpr std::string_view DELETE_ALL =
		R"(DELETE FROM [Skills];)"sv;
	static constexpr std::string_view QUERY_SKILL_FOR_CATEGORY_AND_TYPE =
		R"(SELECT 
			[SkillId] 
		FROM [Skills] 
		WHERE 
			[Category]={} 
			AND [Type]={})"sv;
	static constexpr std::string_view QUERY_SKILLS_FOR_CATEGORY =
		R"(SELECT
			[SkillId]
		FROM [Skills]
		WHERE
			[Category]={};)"sv;
	static constexpr std::string_view QUERY_ALL =
		R"(SELECT
			[SkillId]
		FROM [Skills];)"sv;

	static const std::string_view FIELD_MAXIMUM_TYPE = "MaximumType";
	static const std::string_view FIELD_CATEGORY = "Category";
	static const std::string_view FIELD_TYPE = "Type";
	static const std::string_view FIELD_NAME = "Name";
	static const std::string_view FIELD_SKILL_ID = "SkillId";
	static const std::string_view FIELD_DEFAULT_VALUE = "DefaultValue";

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
			FIELD_DEFAULT_VALUE);
	}

	void Skill::Clear()
	{
		Initialize();
		Common::Execute(DELETE_ALL);
	}

	std::vector<int> Skill::ReadSkillsForCategory(int category)
	{
		std::vector<int> result;
		auto records = Common::Execute(QUERY_SKILLS_FOR_CATEGORY, category);
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_SKILL_ID));
		return result;
	}

	std::vector<int> Skill::All()
	{
		std::vector<int> result;
		auto records = Common::Execute(QUERY_ALL);
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_SKILL_ID));
		return result;
	}
}