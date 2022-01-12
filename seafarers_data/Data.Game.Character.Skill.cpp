#include <algorithm>
#include <Common.Data.h>
#include "Data.Game.Character.h"
#include "Data.Game.Character.Skill.h"
#include "Data.Game.Common.h"
#include "Data.Game.Skill.h"
#include <iterator>
namespace data::game::character
{
	using namespace std::string_view_literals;
	static constexpr std::string_view CREATE_TABLE = 
		R"(CREATE TABLE IF NOT EXISTS [CharacterSkills]
		(
			[CharacterId] INT NOT NULL,
			[SkillId] INT NOT NULL,
			[Value] INT NOT NULL, 
			UNIQUE([CharacterId],[SkillId]),
			FOREIGN KEY ([CharacterId]) REFERENCES [Characters]([CharacterId]),
			FOREIGN KEY ([SkillId]) REFERENCES [Skills]([SkillId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [CharacterSkills]
		(
			[CharacterId],
			[SkillId],
			[Value]
		) 
		VALUES({},{},{});)"sv;
	static constexpr std::string_view QUERY_ITEM =
		R"(SELECT 
			[Value] 
		FROM [CharacterSkills] 
		WHERE 
			[CharacterId]={} 
			AND [SkillId]={};)"sv;
	static constexpr std::string_view QUERY_ITEMS =
		R"(SELECT 
			[SkillId],
			[Value] 
		FROM [CharacterSkills] 
		WHERE 
			[CharacterId]={};)"sv;

	static constexpr std::string_view FIELD_VALUE = "Value";
	static constexpr std::string_view FIELD_SKILL_ID = "SkillId";

	void Skill::Initialize()
	{
		Character::Initialize();
		data::game::Skill::Initialize();
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