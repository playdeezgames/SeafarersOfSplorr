#include <Common.Data.h>
#include "Data.Game.Skill.h"
#include "Data.Game.SkillCharacteristicCoefficient.h"
#include "Data.Game.Common.h"
namespace data::game
{
	using namespace std::string_view_literals;

	static constexpr std::string_view CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [SkillCharacteristicCoefficients]
		(
			[SkillId] INT NOT NULL,
			[Characteristic] INT NOT NULL,
			[Coefficient] INT NOT NULL,
			UNIQUE([SkillId],[Characteristic]),
			FOREIGN KEY ([SkillId]) REFERENCES [Skills]([SkillId])
		);)"sv;
	static constexpr std::string_view REPLACE_ITEM =
		R"(REPLACE INTO [SkillCharacteristicCoefficients]
		(
			[SkillId],
			[Characteristic],
			[Coefficient]
		) 
		VALUES({},{},{});)"sv;

	void SkillCharacteristicCoefficient::Initialize()
	{
		Skill::Initialize();
		Common::Execute(CREATE_TABLE);
	}

	void SkillCharacteristicCoefficient::Write(int skillId, int characteristic, int coefficient)
	{
		Initialize();
		Common::Execute(REPLACE_ITEM, skillId, characteristic, coefficient);
	}
}