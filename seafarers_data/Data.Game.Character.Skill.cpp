#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Skill.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_literals;
	static const std::string CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterSkills]
		(
			[CharacterId] INT NOT NULL,
			[SkillId] INT NOT NULL,
			[Value] INT NOT NULL, 
			UNIQUE([CharacterId],[SkillId]),
			FOREIGN KEY ([CharacterId]) REFERENCE [Characters]([CharacterId])
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

	void Skill::Initialize()
	{
		Character::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void Skill::Write(int characterId, int skillId, int value)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, characterId, skillId, value);
	}

	std::optional<int> Skill::Read(int characterId, int skillId)
	{
		Initialize();
		return Common::TryToInt(
			Common::TryExecuteForOne(QUERY_ITEM, characterId, skillId), 
			FIELD_VALUE);
	}

	std::map<int, int> Skill::Read(int characterId)
	{
		Initialize();
		std::map<int, int> result;
		auto records = Common::Execute(QUERY_ITEMS, characterId);
		std::transform(
			records.begin(),
			records.end(),
			std::inserter(result, result.end()),
			[](const Common::Record& record) 
			{
				return std::make_pair(
					Common::ToInt(record, FIELD_SKILL_ID),
					Common::ToInt(record, FIELD_VALUE));
			});
		return result;
	}
}