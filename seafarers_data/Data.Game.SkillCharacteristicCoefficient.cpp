#include <Common.Data.h>
#include "Data.Game.Skill.h"
#include "Data.Game.SkillCharacteristicCoefficient.h"
#include "Data.Game.Common.h"
namespace data::game
{
	using namespace std::string_literals;

	const static std::string CREATE_TABLE =
		R"(CREATE TABLE IF NOT EXISTS [SkillCharacteristicCoefficients]
		(
			[SkillId] INT NOT NULL,
			[Characteristic] INT NOT NULL,
			[Coefficient] INT NOT NULL,
			UNIQUE([SkillId],[Characteristic]),
			FOREIGN KEY ([SkillId]) REFERENCES [Skills]([SkillId])
		);)"s;
	const static std::string REPLACE_ITEM =
		R"(REPLACE INTO [SkillCharacteristicCoefficients]
		(
			[SkillId],
			[Characteristic],
			[Coefficient]
		) 
		VALUES({},{},{});)"s;

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