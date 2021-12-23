#include <Common.Data.h>
#include "Data.Game.Character.Skill.h"
#include "Data.Game.Common.h"
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterSkills]
		(
			[CharacterId] INT NOT NULL,
			[SkillId] INT NOT NULL,
			[Value] INT NOT NULL, 
			UNIQUE([CharacterId],[SkillId])
		);)"s;
	static const std::string REPLACE_ITEM = 
		R"(REPLACE INTO [CharacterSkills]
		(
			[CharacterId],
			[SkillId],
			[Value]
		) 
		VALUES({},{},{});)"s;
	static const std::string QUERY_ITEM = 
		R"(SELECT 
			[Value] 
		FROM [CharacterSkills] 
		WHERE 
			[CharacterId]={} 
			AND [SkillId]={};)"s;
	static const std::string QUERY_ITEMS = 
		R"(SELECT 
			[SkillId],
			[Value] 
		FROM [CharacterSkills] 
		WHERE 
			[CharacterId]={};)"s;

	static const std::string FIELD_VALUE = "Value";
	static const std::string FIELD_SKILL_ID = "SkillId";
	static const auto AutoCreateTable = data::game::Common::Run(CREATE_TABLE);

	void Skill::Write(int characterId, int skillId, int value)
	{
		AutoCreateTable();
		Common::Execute(REPLACE_ITEM, characterId, skillId, value);
	}

	std::optional<int> Skill::Read(int characterId, int skillId)
	{
		AutoCreateTable();
		auto records = Common::Execute(QUERY_ITEM, characterId, skillId);
		if (!records.empty())
		{
			return common::Data::ToOptionalInt(records.front()[FIELD_VALUE]);
		}
		return std::nullopt;
	}

	std::map<int, int> Skill::Read(int characterId)
	{
		AutoCreateTable();
		std::map<int, int> result;
		auto records = Common::Execute(QUERY_ITEMS, characterId);
		for (auto record : records)
		{
			result[common::Data::ToInt(record[FIELD_SKILL_ID])] = 
				common::Data::ToInt(record[FIELD_VALUE]);
		}
		return result;
	}
}